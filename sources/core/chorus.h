//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "ensemble_chorus.h"
#include <memory>

class Chorus {
public:
    Chorus();
    ~Chorus();

    void setup(float samplerate, unsigned bufsize);
    void process(float *inout[2], unsigned nframes, ec_channel_layout ecc, const unsigned line_routing[6]);

    unsigned id() const;
    void id(unsigned i);

    void delay(float r_delay);
    void regen(float r_regen);
    void delays(const float phases[], const unsigned nums[], unsigned count);

    void nstages(unsigned n);
    void mod_range(float r);
    void slow_rate(float r);
    void fast_rate(float r);

    static constexpr unsigned lfo_wave_count = EC_LFO_WAVE_COUNT;
    void slow_wave(unsigned w);
    void fast_wave(unsigned w);

    void slow_rand(float r);
    void fast_rand(float r);

    void modulation_depth(unsigned lfo, float depth);

    void lpf(float r_cutoff, float r_q);

    float current_slow_modulation(unsigned lfo) const;
    float current_fast_modulation(unsigned lfo) const;

private:
    struct Impl;
    std::unique_ptr<Impl> P;
};
