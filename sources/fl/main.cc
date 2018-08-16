//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "main_view.h"
#include "main_controller.h"
#include "message_queue.h"
#include "messages.h"
#include <ensemble_chorus.h>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <RtAudio.h>
#include <getopt.h>
#include <jsl/math>
#include <bitset>
#include <memory>
#include <thread>
#include <system_error>
#include <cstring>
#include <cmath>
#include <cassert>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#if !defined(_WIN32)
#include <sys/mman.h>
#endif

struct Audio_Context {
    chorus_u effect;
    std::unique_ptr<Message_Queue> mq_to_effect;
    std::unique_ptr<Message_Queue> mq_to_user;
    std::unique_ptr<uint8_t[]> msg_buffer;
    std::unique_ptr<RtAudio> audiosys;
    std::bitset<EC_PARAMETER_COUNT> notification_mask;
};

static unsigned audiobufsize = 1024;
static constexpr unsigned audiochannels = 2;

static bool setup_audio(Audio_Context &ctx);
static int process_audio(void *output, void *input, unsigned nframes, double time, RtAudioStreamStatus status, void *userdata);
static void process_message(const Basic_Message &msg, Audio_Context &ctx);

static void update_callback(void *userdata);
static constexpr float update_interval = 10e-3;

#if !defined(_WIN32)
static void handle_signals();
static int signals_fds[2];
#endif

int main(int argc, char *argv[])
{
    Audio_Context ctx;

    for (int c; (c = getopt(argc, argv, "b:")) != -1;) {
        switch (c) {
        case 'b': {
            unsigned count;
            if (sscanf(optarg, "%u%n", &::audiobufsize, &count) != 1 ||
                strlen(optarg) != count || ::audiobufsize <= 0)
                return 1;
            break;
        }
        default:
            return 1;
        }
    }
    if (argc - optind != 0)
        return 1;

#if !defined(_WIN32)
    handle_signals();
#endif

    if (!setup_audio(ctx))
        return 1;

    int w = Main_View::W;
    int h = Main_View::H;

    Fl_Double_Window *win = new Fl_Double_Window(w, h);
    win->label("JPC Ensemble Chorus");

    Main_Controller ctrl(*ctx.mq_to_user, *ctx.mq_to_effect);
    Main_View *view = new Main_View(0, 0, w, h);
    view->controller(&ctrl);
    ctrl.request_parameters();

    win->show();

    bool run = true;

#if !defined(_WIN32)
    Fl::add_fd(
        ::signals_fds[0], FL_READ,
        +[](int, void *userdata)
        {
            *reinterpret_cast<bool *>(userdata) = false;
            fprintf(stderr, "Interrupt\n");
        },
        &run);
#endif

    Fl::add_timeout(update_interval, &update_callback, &ctrl);

    while (run && Fl::wait() != 0);
    return 0;
}

static bool setup_audio(Audio_Context &ctx)
{
#if !defined(_WIN32)
    if (mlockall(MCL_CURRENT|MCL_FUTURE) != 0)
        fprintf(stderr, "Could not lock memory\n");
#endif

    RtAudio *audiosys = new RtAudio(RtAudio::Api::UNSPECIFIED);
    ctx.audiosys.reset(audiosys);

    RtAudio::StreamParameters in_param;
    RtAudio::StreamParameters out_param;

    in_param.deviceId = audiosys->getDefaultInputDevice();
    in_param.nChannels = audiochannels;
    out_param.deviceId = audiosys->getDefaultOutputDevice();
    out_param.nChannels = audiochannels;

    RtAudio::StreamOptions opts;
    opts.streamName = "JPC Ensemble Chorus";
    opts.flags = RTAUDIO_NONINTERLEAVED|RTAUDIO_SCHEDULE_REALTIME|RTAUDIO_ALSA_USE_DEFAULT|RTAUDIO_JACK_DONT_CONNECT;

    unsigned samplerate = audiosys->getDeviceInfo(in_param.deviceId).preferredSampleRate;
    audiosys->openStream(
        &out_param, &in_param, RTAUDIO_FLOAT32, samplerate,
        &::audiobufsize, &process_audio, &ctx, &opts);

    chorus_t *effect = ensemble_chorus_alloc();
    if (!effect)
        throw std::bad_alloc();
    ctx.effect.reset(effect);

    ctx.mq_to_effect.reset(new Message_Queue(1024));
    ctx.mq_to_user.reset(new Message_Queue(1024));
    ctx.msg_buffer.reset(Messages::allocate_buffer());

    if (!ensemble_chorus_init(effect, samplerate, ::audiobufsize))
        return false;

    audiosys->startStream();
    return true;
}

static int process_audio(void *output_, void *input_, unsigned nframes, double time, RtAudioStreamStatus status, void *userdata)
{
    jsl::denormal_disabler dd;

    Audio_Context &ctx = *(Audio_Context *)userdata;
    chorus_t *effect = ctx.effect.get();

    Message_Queue &mq_in = *ctx.mq_to_effect.get();
    Message_Queue &mq_out = *ctx.mq_to_user.get();

    auto &msg = *reinterpret_cast<Basic_Message *>(ctx.msg_buffer.get());
    while (mq_in.read_message(msg))
        process_message(msg, ctx);

    float *output = reinterpret_cast<float *>(output_);
    const float *input = reinterpret_cast<float *>(input_);

    std::memcpy(output, input, nframes * audiochannels * sizeof(float));

    float *bufs[audiochannels];
    for (unsigned i = 0; i < audiochannels; ++i)
        bufs[i] = output + i * nframes;

    ensemble_chorus_process(effect, bufs, nframes);

    std::bitset<EC_PARAMETER_COUNT> &nm = ctx.notification_mask;
    for (unsigned i = 0; i < EC_PARAMETER_COUNT; ++i) {
        if (nm.test(i)) {
            Messages::NotifyParameter msg;
            msg.id = i;
            msg.value = ensemble_chorus_get_parameter(effect, (ec_parameter)i);
            if (!mq_out.write_message(msg))
                break;
            nm.reset(i);
        }
    }

    {
        Messages::NotifyModulation msg;
        ensemble_chorus_get_current_modulation(effect, msg.slow, msg.fast);
        mq_out.write_message(msg);
    }

    return 0;
}

static void process_message(const Basic_Message &msg, Audio_Context &ctx)
{
    chorus_t *effect = ctx.effect.get();

    switch (msg.tag) {
    case Message_Tag::RequestNotifyParameters: {
        ctx.notification_mask.set();
        break;
    }
    case Message_Tag::RequestSetParameter: {
        auto &body = static_cast<const Messages::RequestSetParameter &>(msg);
        ensemble_chorus_set_parameter(effect, (ec_parameter)body.id, body.value);
        break;
    }
    default:
        assert(false);
    }
}

static void update_callback(void *userdata)
{
    auto &ctrl = *reinterpret_cast<Main_Controller *>(userdata);
    ctrl.handle_messages();
    Fl::repeat_timeout(update_interval, &update_callback, userdata);
}

#if !defined(_WIN32)
static void set_cloexec(int fd)
{
    int flags = fcntl(fd, F_GETFD);
    if (flags == -1)
        throw std::system_error(errno, std::generic_category(), "fcntl");
    if (fcntl(fd, F_SETFD, flags|FD_CLOEXEC) == -1)
        throw std::system_error(errno, std::generic_category(), "fcntl");
}

static void handle_signals()
{
    bool success = false;

    if (pipe(::signals_fds) != 0)
        throw std::system_error(errno, std::generic_category(), "pipe");

    set_cloexec(::signals_fds[0]);
    set_cloexec(::signals_fds[1]);

    sigset_t sigs;
    int nsignal = 0;

    sigemptyset(&sigs);
    for (int signo : {SIGINT, SIGTERM}) {
        sigaddset(&sigs, signo);
        nsignal = signo + 1;
    }

    for (int sig = 1; sig < nsignal; ++sig) {
        if (!sigismember(&sigs, sig))
            continue;
        struct sigaction sa = {};
        sa.sa_handler = +[](int) { write(::signals_fds[1], "", 1); };
        sa.sa_mask = sigs;
        if (sigaction(sig, &sa, nullptr) == -1)
            throw std::system_error(errno, std::generic_category(), "sigaction");
    }

    std::thread handler_thread([]() { for (;;) pause(); });
    handler_thread.detach();

    if (pthread_sigmask(SIG_BLOCK, &sigs, nullptr) == -1)
        throw std::system_error(errno, std::generic_category(), "pthread_sigmask");

    success = true;
}
#endif
