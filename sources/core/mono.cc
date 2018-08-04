//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mono.h"
#include "bbd.h"
#include "lfos.h"
#include "dsp.h"
#include <DspFilters/RBJ.h>
#include <memory>
#include <cstring>
#include <cassert>

static constexpr unsigned max_delay_lines = 6;
static constexpr float min_clock_rate = 1500;
static constexpr float max_clock_rate = 22050/*100000*/; // TODO high clock rates
static constexpr float clock_mod_depth_range = 0.25;  // in Hz/stages
static constexpr float chorus_max_delay = 100e-3;
static constexpr float lfo_slow_freq_min = 0.01;
static constexpr float lfo_slow_freq_max = 2.0;
static constexpr float lfo_fast_freq_min = 2.0;
static constexpr float lfo_fast_freq_max = 10.0;
static constexpr float lowpass_cutoff_min = 100.0;
static constexpr float lowpass_cutoff_max = 20e3;
static constexpr float lowpass_q_min = 0.05;
static constexpr float lowpass_q_max = 1.0;

struct Mono_Chorus::Impl {
    unsigned id_ = 0;

    float samplerate_ = 0;
    unsigned bufsize_ = 0;

    unsigned num_delay_lines_ = 0;
    float lfo_slow_freq_ = 0;
    float lfo_fast_freq_ = 0;
    float clock_freq_ = 0;
    float clock_mod_range_ = 0;
    float clock_mod_depth_[max_delay_lines] = {};
    float delay_ = 0;
    unsigned nstages_ = 0;

    BBD_Clock clock_[max_delay_lines];
    BBD_Line delay_line_[max_delay_lines];
    LFOs lfos_slow_;
    LFOs lfos_fast_;

    Dsp::SimpleFilter<Dsp::RBJ::LowPass, 1> lowpass_;

    std::unique_ptr<float[]> tmpbuf_;

    void update_clock_freq();
};

Mono_Chorus::Mono_Chorus()
    : P(new Impl)
{
}

Mono_Chorus::~Mono_Chorus()
{
}

void Mono_Chorus::setup(float samplerate, unsigned bufsize)
{
    Impl &P = *this->P;

    P.samplerate_ = samplerate;
    P.bufsize_ = bufsize;

    float delay = 0.1;
    unsigned nstages = 2048;
    unsigned num_delay_lines = 3;
    float lfo_slow_freq = 1.0;
    float lfo_fast_freq = 6.0;

    P.num_delay_lines_ = num_delay_lines;
    P.lfo_slow_freq_ = lfo_slow_freq;
    P.lfo_fast_freq_ = lfo_fast_freq;
    P.clock_mod_range_ = 1.0;
    for (unsigned i = 0; i < max_delay_lines; ++i)
        P.clock_mod_depth_[i] = 1.0;
    P.delay_ = delay;
    P.nstages_ = nstages;
    P.update_clock_freq();

    for (unsigned i = 0; i < max_delay_lines; ++i) {
        BBD_Line &line = P.delay_line_[i];
        line.setup(samplerate, max_clock_rate, nstages);
    }

    LFOs &lfos_slow = P.lfos_slow_;
    LFOs &lfos_fast = P.lfos_fast_;
    lfos_slow.setup(samplerate, bufsize, max_delay_lines);
    lfos_fast.setup(samplerate, bufsize, max_delay_lines);

    float phases[max_delay_lines];
    for (unsigned i = 0; i < num_delay_lines; ++i)
        phases[i] = i / (float)num_delay_lines;
    lfos_slow.phases(phases, num_delay_lines);
    lfos_fast.phases(phases, num_delay_lines);

    auto &lowpass = P.lowpass_;
    float lowpass_cutoff = lowpass_cutoff_max;
    float lowpass_q = M_SQRT1_2;
    lowpass.setup(samplerate, lowpass_cutoff, lowpass_q);

    P.tmpbuf_.reset(new float[(3 + 2 * max_delay_lines) * bufsize]);

#pragma message("XXX remove")
    if (true && P.id_ == 0) {
        for (unsigned i = 0; i < BBD_Line::supported_nstages_count; ++i) {
            unsigned ns = BBD_Line::nstages_min << i;
            float min_delay = BBD_Clock::delay_for_hz_rate(max_clock_rate, ns);
            float max_delay = BBD_Clock::delay_for_hz_rate(min_clock_rate, ns);
            fprintf(stderr, "stages=%u min_delay=%g max_delay=%g\n", ns, min_delay, max_delay);
        }
    }
}

void Mono_Chorus::process(float *inout, unsigned nframes)
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

    float *tmp_in = tmp; tmp += bufsize;
    float *tmp_out = tmp; tmp += bufsize;
    float *tmp_clock = tmp; tmp += bufsize;

    std::memcpy(tmp_in, inout, nframes * sizeof(float));
    std::memset(inout, 0, nframes * sizeof(float));

    LFOs &lfos_slow = P.lfos_slow_;
    LFOs &lfos_fast = P.lfos_fast_;
    float *lfo_slow_outs[max_delay_lines];
    float *lfo_fast_outs[max_delay_lines];
    for (unsigned d = 0; d < num_delay_lines; ++d) {
        lfo_slow_outs[d] = tmp; tmp += bufsize;
        lfo_fast_outs[d] = tmp; tmp += bufsize;
    }

    float lfo_slow_freq = P.lfo_slow_freq_;
    float lfo_fast_freq = P.lfo_fast_freq_;
    lfos_slow.perform(lfo_slow_freq, lfo_slow_outs, nframes);
    lfos_fast.perform(lfo_fast_freq, lfo_fast_outs, nframes);

    for (unsigned d = 0; d < num_delay_lines; ++d) {
        BBD_Clock &clock = P.clock_[d];
        BBD_Line &line = P.delay_line_[d];
        float *lfo_slow_out = lfo_slow_outs[d];
        float *lfo_fast_out = lfo_fast_outs[d];

        float mod_depth = clock_mod_range * clock_mod_depth[d] * clock_mod_depth_range * nstages;
        for (unsigned i = 0; i < nframes; ++i) {
            float clock_mod = (lfo_slow_out[i] - 0.5) + (lfo_fast_out[i] - 0.5);
            float clock_f = clock_freq + clock_mod * mod_depth;
            tmp_clock[i] = clock.tick(clock_f * ts);
        }

        std::memcpy(tmp_out, tmp_in, nframes * sizeof(float));
        line.process(nframes, tmp_out, tmp_clock);

        for (unsigned i = 0; i < nframes; ++i)
            inout[i] += tmp_out[i];
    }

    auto &lowpass = P.lowpass_;
    dsp::process_mono_DspFilter(lowpass, inout, nframes);
}

unsigned Mono_Chorus::id() const
{
    const Impl &P = *this->P;
    return P.id_;
}

void Mono_Chorus::id(unsigned i)
{
    Impl &P = *this->P;
    P.id_ = i;
}

void Mono_Chorus::delay(float r_delay)
{
    Impl &P = *this->P;
    unsigned nstages = P.nstages_;

    float min_delay = BBD_Clock::delay_for_hz_rate(max_clock_rate, nstages);
    float max_delay = BBD_Clock::delay_for_hz_rate(min_clock_rate, nstages);
    max_delay = std::min(max_delay, chorus_max_delay);
    float delay = min_delay + r_delay * (max_delay - min_delay);

    P.delay_ = delay;
    P.update_clock_freq();
}

void Mono_Chorus::delays(const float *phases, unsigned count)
{
    assert(count <= max_delay_lines);

    Impl &P = *this->P;
    LFOs &lfos_slow = P.lfos_slow_;
    LFOs &lfos_fast = P.lfos_fast_;

    P.num_delay_lines_ = count;
    lfos_slow.phases(phases, count);
    lfos_fast.phases(phases, count);
}

void Mono_Chorus::nstages(unsigned n)
{
    Impl &P = *this->P;
    for (unsigned d = 0; d < max_delay_lines; ++d) {
        BBD_Line &line = P.delay_line_[d];
        line.nstages(d);
    }
    P.nstages_ = n;
    P.update_clock_freq();
}

void Mono_Chorus::mod_range(float r)
{
    Impl &P = *this->P;
    P.clock_mod_range_ = 2 * r;
}

void Mono_Chorus::slow_rate(float r)
{
    Impl &P = *this->P;
    P.lfo_slow_freq_ = lfo_slow_freq_min + r * (lfo_slow_freq_max - lfo_slow_freq_min);
}

void Mono_Chorus::fast_rate(float r)
{
    Impl &P = *this->P;
    P.lfo_fast_freq_ = lfo_fast_freq_min + r * (lfo_fast_freq_max - lfo_fast_freq_min);
}

void Mono_Chorus::slow_wave(unsigned w)
{
    Impl &P = *this->P;
    P.lfos_slow_.shape((LFOs::Shape)w);
}

void Mono_Chorus::fast_wave(unsigned w)
{
    Impl &P = *this->P;
    P.lfos_fast_.shape((LFOs::Shape)w);
}

void Mono_Chorus::slow_rand(float r)
{
    Impl &P = *this->P;
    P.lfos_slow_.random(100.0f * r);
}

void Mono_Chorus::fast_rand(float r)
{
    Impl &P = *this->P;
    P.lfos_fast_.random(100.0f * r);
}

void Mono_Chorus::modulation_depth(unsigned lfo, float depth)
{
    Impl &P = *this->P;
    P.clock_mod_depth_[lfo] = depth;
}

void Mono_Chorus::lpf(float r_cutoff, float r_q)
{
    Impl &P = *this->P;
    auto &lowpass = P.lowpass_;
    float cutoff = lowpass_cutoff_min + r_cutoff * (lowpass_cutoff_max - lowpass_cutoff_min);
    float q = lowpass_q_min + r_q * (lowpass_q_max - lowpass_q_min);
    lowpass.setup(P.samplerate_, cutoff, q);
}

float Mono_Chorus::current_slow_modulation(unsigned lfo) const
{
    const Impl &P = *this->P;
    return P.lfos_slow_.last_value()[lfo];
}

float Mono_Chorus::current_fast_modulation(unsigned lfo) const
{
    const Impl &P = *this->P;
    return P.lfos_fast_.last_value()[lfo];
}

void Mono_Chorus::Impl::update_clock_freq()
{
    float delay = delay_;
    unsigned nstages = nstages_;

    float clock_hz = BBD_Clock::hz_rate_for_delay(delay, nstages);
    clock_freq_ = clock_hz;

#pragma message("XXX remove")
    if (false)
        fprintf(stderr, "Clock frequency %g for delay=%g nstages=%u\n", clock_hz, delay, nstages);
}
