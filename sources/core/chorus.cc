//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "chorus.h"
#include "bbd_line.h"
#include "lfos.h"
#include "dsp.h"
#include <DspFilters/RBJ.h>
#include <DspFilters/SmoothedFilter.h>
#include <memory>
#include <cstring>
#include <cassert>

static constexpr float min_clock_rate = 1500;
static constexpr float max_clock_rate = 22050/*100000*/; // TODO high clock rates
static constexpr float clock_mod_depth_range = 0.25;  // in Hz/stages
static constexpr float chorus_max_delay = 100e-3;
static constexpr float lfo_slow_freq_min = 0.01;
static constexpr float lfo_slow_freq_max = 2.0;
static constexpr float lfo_fast_freq_min = 2.0;
static constexpr float lfo_fast_freq_max = 10.0;
static constexpr float lowpass_q_min = 0.05;
static constexpr float lowpass_q_max = 1.0;

struct Chorus::Impl {
    Impl() : lpf_{SmoothLPF(1024), SmoothLPF(1024)} {}

    unsigned id_ = 0;

    float samplerate_ = 0;
    unsigned bufsize_ = 0;

    unsigned num_delay_lines_ = 0;
    unsigned id_delay_line_[6] = {};
    float lfo_slow_freq_ = 0;
    float lfo_fast_freq_ = 0;
    float clock_freq_ = 0;
    float clock_mod_range_ = 0;
    float clock_mod_depth_[6] = {};
    float delay_ = 0;
    float r_delay_ = 0;
    unsigned nstages_ = 0;

    BBD_Line delay_line_[6];
    LFOs lfos_slow_;
    LFOs lfos_fast_;

    typedef Dsp::SmoothedFilterDesign<Dsp::RBJ::Design::LowPass, 2> SmoothLPF;
    SmoothLPF lpf_[2];

    std::unique_ptr<float[]> tmpbuf_;

    void update_clock_freq();
};

Chorus::Chorus()
    : P(new Impl)
{
}

Chorus::~Chorus()
{
}

void Chorus::setup(float samplerate, unsigned bufsize)
{
    Impl &P = *this->P;

    P.samplerate_ = samplerate;
    P.bufsize_ = bufsize;

    float r_delay = 0.5;
    unsigned nstages = 2048;
    unsigned num_delay_lines = 3;
    float lfo_slow_freq = 1.0;
    float lfo_fast_freq = 6.0;

    P.num_delay_lines_ = num_delay_lines;
    P.lfo_slow_freq_ = lfo_slow_freq;
    P.lfo_fast_freq_ = lfo_fast_freq;
    P.clock_mod_range_ = 1.0;
    for (unsigned i = 0; i < 6; ++i)
        P.clock_mod_depth_[i] = 1.0;
    P.nstages_ = nstages;
    delay(r_delay);

    for (unsigned i = 0; i < 6; ++i) {
        BBD_Line &line = P.delay_line_[i];
        line.setup(samplerate, nstages, bbd_fin_j60, bbd_fout_j60);
    }

    LFOs &lfos_slow = P.lfos_slow_;
    LFOs &lfos_fast = P.lfos_fast_;
    lfos_slow.setup(samplerate, bufsize, 6);
    lfos_fast.setup(samplerate, bufsize, 6);

    float phases[6];
    for (unsigned i = 0; i < num_delay_lines; ++i)
        phases[i] = i / (float)num_delay_lines;
    lfos_slow.phases(phases, num_delay_lines);
    lfos_fast.phases(phases, num_delay_lines);

    float lowpass_cutoff = ensemble_chorus_parameter_default(ECP_LPF_CUTOFF);
    float lowpass_q = ensemble_chorus_parameter_default(ECP_LPF_Q);
    lpf(lowpass_cutoff, lowpass_q);

    P.tmpbuf_.reset(new float[(4 + 2 * 6) * bufsize]);

#pragma message("XXX remove")
    if (true && P.id_ == 0) {
        for (unsigned i = 0; i < BBD_Line::supported_nstages_count; ++i) {
            unsigned ns = BBD_Line::nstages_min << i;
            float min_delay = BBD_Line::delay_for_hz_rate(max_clock_rate, ns);
            float max_delay = BBD_Line::delay_for_hz_rate(min_clock_rate, ns);
            fprintf(stderr, "stages=%u min_delay=%g max_delay=%g\n", ns, min_delay, max_delay);
        }
    }
}

void Chorus::process(float *inout[2], unsigned nframes, ec_channel_layout ecc, const unsigned line_routing[6])
{
    Impl &P = *this->P;

    float fs = P.samplerate_;
    float ts = 1 / fs;
    unsigned bufsize = P.bufsize_;
    unsigned num_delay_lines = P.num_delay_lines_;
    float clock_freq = P.clock_freq_;
    float clock_mod_range = P.clock_mod_range_;
    const float *clock_mod_depth = P.clock_mod_depth_;
    unsigned nstages = P.nstages_;
    float *tmp = P.tmpbuf_.get();

    float *tmp_in[2] = {tmp, tmp + bufsize};
    tmp += 2 * bufsize;
    float *tmp_out = tmp; tmp += bufsize;
    float *tmp_clock = tmp; tmp += bufsize;

    for (unsigned c = 0; c < 2; ++c) {
        float *channel_inout = inout[c];
        std::memcpy(tmp_in[c], channel_inout, nframes * sizeof(float));
        std::memset(channel_inout, 0, nframes * sizeof(float));
    }

    LFOs &lfos_slow = P.lfos_slow_;
    LFOs &lfos_fast = P.lfos_fast_;
    float *lfo_slow_outs[6];
    float *lfo_fast_outs[6];
    for (unsigned d = 0; d < num_delay_lines; ++d) {
        lfo_slow_outs[d] = tmp; tmp += bufsize;
        lfo_fast_outs[d] = tmp; tmp += bufsize;
    }

    float lfo_slow_freq = P.lfo_slow_freq_;
    float lfo_fast_freq = P.lfo_fast_freq_;
    lfos_slow.perform(lfo_slow_freq, lfo_slow_outs, nframes);
    lfos_fast.perform(lfo_fast_freq, lfo_fast_outs, nframes);

    ///
    enum { L = 1, R = 2 };
    const unsigned mono_input_channel[6] =
        {L|R, L|R, L|R, L|R, L|R, L|R};
    const unsigned stereo_input_channel[6] =
        {L, L|R, R, L, L|R, R};
    ///
    const unsigned *input_channel = (ecc == ECC_STEREO) ?
        stereo_input_channel : mono_input_channel;

    for (unsigned d = 0; d < num_delay_lines; ++d) {
        unsigned id = P.id_delay_line_[d];
        BBD_Line &line = P.delay_line_[d];
        float *lfo_slow_out = lfo_slow_outs[d];
        float *lfo_fast_out = lfo_fast_outs[d];

        float mod_depth = clock_mod_range * clock_mod_depth[d] * clock_mod_depth_range * nstages;
        for (unsigned i = 0; i < nframes; ++i) {
            float clock_mod = (lfo_slow_out[i] - 0.5) + (lfo_fast_out[i] - 0.5);
            float clock_f = clock_freq + clock_mod * mod_depth;
            tmp_clock[i] = clock_f * ts;
        }

        std::memset(tmp_out, 0, nframes * sizeof(float));

        float input_gain = (input_channel[id] == (L|R)) ? M_SQRT1_2 : 1.0;
        if (input_channel[id] & L) {
            const float *channel_inout = tmp_in[0];
            for (unsigned i = 0; i < nframes; ++i)
                tmp_out[i] += input_gain * channel_inout[i];
        }
        if (input_channel[id] & R) {
            const float *channel_inout = tmp_in[1];
            for (unsigned i = 0; i < nframes; ++i)
                tmp_out[i] += input_gain * channel_inout[i];
        }

        line.process(nframes, tmp_out, tmp_clock);

        float routing_gain = (line_routing[id] == (L|R)) ? M_SQRT1_2 : 1.0;
        if (line_routing[id] & L) {
            float *channel_inout = inout[0];
            for (unsigned i = 0; i < nframes; ++i)
                channel_inout[i] += routing_gain * tmp_out[i];
        }
        if (line_routing[id] & R) {
            float *channel_inout = inout[1];
            for (unsigned i = 0; i < nframes; ++i)
                channel_inout[i] += routing_gain * tmp_out[i];
        }
    }

    for (Impl::SmoothLPF &lpf : P.lpf_)
        lpf.process(nframes, inout);
}

unsigned Chorus::id() const
{
    const Impl &P = *this->P;
    return P.id_;
}

void Chorus::id(unsigned i)
{
    Impl &P = *this->P;
    P.id_ = i;
}

void Chorus::delay(float r_delay)
{
    Impl &P = *this->P;
    unsigned nstages = P.nstages_;

    float min_delay = BBD_Line::delay_for_hz_rate(max_clock_rate, nstages);
    float max_delay = BBD_Line::delay_for_hz_rate(min_clock_rate, nstages);
    max_delay = std::min(max_delay, chorus_max_delay);
    float delay = min_delay + r_delay * (max_delay - min_delay);

    P.delay_ = delay;
    P.r_delay_ = r_delay;
    P.update_clock_freq();
}

void Chorus::delays(const float phases[], const unsigned nums[], unsigned count)
{
    assert(count <= 6);

    Impl &P = *this->P;
    LFOs &lfos_slow = P.lfos_slow_;
    LFOs &lfos_fast = P.lfos_fast_;

    P.num_delay_lines_ = count;
    for (unsigned i = 0; i < count; ++i)
        P.id_delay_line_[i] = nums[i];
    lfos_slow.phases(phases, count);
    lfos_fast.phases(phases, count);
}

void Chorus::nstages(unsigned n)
{
    Impl &P = *this->P;
    for (unsigned d = 0; d < 6; ++d) {
        BBD_Line &line = P.delay_line_[d];
        line.nstages(d);
    }
    P.nstages_ = n;
    delay(P.r_delay_);
}

void Chorus::mod_range(float r)
{
    Impl &P = *this->P;
    P.clock_mod_range_ = 2 * r;
}

void Chorus::slow_rate(float r)
{
    Impl &P = *this->P;
    P.lfo_slow_freq_ = lfo_slow_freq_min + r * (lfo_slow_freq_max - lfo_slow_freq_min);
}

void Chorus::fast_rate(float r)
{
    Impl &P = *this->P;
    P.lfo_fast_freq_ = lfo_fast_freq_min + r * (lfo_fast_freq_max - lfo_fast_freq_min);
}

void Chorus::slow_wave(unsigned w)
{
    Impl &P = *this->P;
    P.lfos_slow_.shape((LFOs::Shape)w);
}

void Chorus::fast_wave(unsigned w)
{
    Impl &P = *this->P;
    P.lfos_fast_.shape((LFOs::Shape)w);
}

void Chorus::slow_rand(float r)
{
    Impl &P = *this->P;
    P.lfos_slow_.random(100.0f * r);
}

void Chorus::fast_rand(float r)
{
    Impl &P = *this->P;
    P.lfos_fast_.random(100.0f * r);
}

void Chorus::modulation_depth(unsigned lfo, float depth)
{
    Impl &P = *this->P;
    P.clock_mod_depth_[lfo] = depth;
}

void Chorus::lpf(float cutoff, float r_q)
{
    Impl &P = *this->P;
    float q = lowpass_q_min + r_q * (lowpass_q_max - lowpass_q_min);

    for (Impl::SmoothLPF &lpf : P.lpf_) {
        Dsp::Params par;
        par[0] = P.samplerate_;
        par[1] = cutoff;
        par[2] = q;
        lpf.setParams(par);
    }
}

void Chorus::aa_cutoff(float cutoff)
{
#warning XXX implement me new model
    // Impl &P = *this->P;
    // P.delay_line_[0].aa_cutoff(cutoff);
    // for (unsigned i = 0; i < 6; ++i)
    //     P.delay_line_[i].aa_reset();
}

float Chorus::current_slow_modulation(unsigned lfo) const
{
    const Impl &P = *this->P;
    return P.lfos_slow_.last_value()[lfo];
}

float Chorus::current_fast_modulation(unsigned lfo) const
{
    const Impl &P = *this->P;
    return P.lfos_fast_.last_value()[lfo];
}

void Chorus::Impl::update_clock_freq()
{
    float delay = delay_;
    unsigned nstages = nstages_;

    float clock_hz = BBD_Line::hz_rate_for_delay(delay, nstages);
    clock_freq_ = clock_hz;

#pragma message("XXX remove")
    if (false)
        fprintf(stderr, "Clock frequency %g for delay=%g nstages=%u\n", clock_hz, delay, nstages);
}
