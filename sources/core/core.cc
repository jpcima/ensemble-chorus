//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ensemble_chorus.h"
#include "mono.h"
#include "bbd.h"
#include "lfos.h"
#include <jsl/math>
#include <memory>
#include <cstring>
#include <cassert>

struct chorus {
    float samplerate_ = 0;
    unsigned channels_ = 1;
    std::unique_ptr<Mono_Chorus[]> chorus_;
    std::unique_ptr<float[]> parameter_;
    std::unique_ptr<float[]> tmpbuf_;
};

chorus_t *ensemble_chorus_alloc()
{
    chorus *ec = new chorus;
    return ec;
}

void ensemble_chorus_free(chorus_t *ec)
{
    delete ec;
}

bool ensemble_chorus_init(chorus_t *ec, float samplerate, unsigned bufsize, unsigned channels)
{
    ec->samplerate_ = samplerate;
    ec->channels_ = channels;

    Mono_Chorus *chorus = new Mono_Chorus[channels];
    ec->chorus_.reset(chorus);
    for (unsigned i = 0; i < channels; ++i) {
        chorus[i].id(i);
        chorus[i].setup(samplerate, bufsize);
    }

    float *parameters = new float[EC_PARAMETER_COUNT]();
    ec->parameter_.reset(parameters);

    parameters[ECP_DELAY] = 0.5;
    parameters[ECP_NSTAGES] = BBD_Line::adjust_nstages(1024);
    parameters[ECP_MOD_RANGE] = 0.5;
    parameters[ECP_SLOW_RATE] = 0.5;
    parameters[ECP_SLOW_WAVE] = (int)LFOs::Shape::Triangle;
    parameters[ECP_SLOW_RAND] = 0.1;
    parameters[ECP_FAST_RATE] = 0.5;
    parameters[ECP_FAST_WAVE] = (int)LFOs::Shape::Triangle;
    parameters[ECP_FAST_RAND] = 0.1;
    parameters[ECP_LPF_CUTOFF] = 1.0;
    parameters[ECP_LPF_Q] = M_SQRT1_2;
    parameters[ECP_GAIN_IN] = 1.0;
    parameters[ECP_GAIN_OUT] = 1.0;
    parameters[ECP_MIX_DRY] = M_SQRT1_2;
    parameters[ECP_MIX_WET] = M_SQRT1_2;

    for (unsigned i = 0, n = 3; i < 6; ++i) {
        parameters[ECP_ENABLE1 + i * (ECP_ENABLE2 - ECP_ENABLE1)] = (i < n) ? 1 : 0;
        parameters[ECP_PHASE1 + i * (ECP_ENABLE2 - ECP_ENABLE1)] = i * (1.0 / n);
        parameters[ECP_DEPTH1 + i * (ECP_ENABLE2 - ECP_ENABLE1)] = 0.5;
    }

    for (unsigned i = 0; i < EC_PARAMETER_COUNT; ++i)
        ensemble_chorus_set_parameter(ec, (ec_parameter)i, parameters[i]);

    ec->tmpbuf_.reset(new float[bufsize]);

    return true;
}

void ensemble_chorus_process(chorus_t *ec, float *inout[], unsigned nframes)
{
    unsigned channels = ec->channels_;
    Mono_Chorus *chorus = ec->chorus_.get();
    const float *parameters = ec->parameter_.get();

    float *tmp = ec->tmpbuf_.get();
    float *chorus_inout = tmp;

    bool bypass = parameters[ECP_BYPASS];
    if (bypass)
        return;

    float input_gain = parameters[ECP_GAIN_IN];
    for (unsigned c = 0; c < channels; ++c) {
        float *channel_inout = inout[c];
        for (unsigned i = 0; i < nframes; ++i)
            channel_inout[i] *= input_gain;
    }

    float output_gain = parameters[ECP_GAIN_OUT];
    float dry_gain = parameters[ECP_MIX_DRY] * output_gain;
    float wet_gain = parameters[ECP_MIX_WET] * output_gain;

    for (unsigned c = 0; c < channels; ++c) {
        Mono_Chorus &mc = chorus[c];
        float *channel_inout = inout[c];

        std::memcpy(chorus_inout, channel_inout, nframes * sizeof(float));
        mc.process(chorus_inout, nframes);

        for (unsigned i = 0; i < nframes; ++i) {
            channel_inout[i] =
                dry_gain * channel_inout[i] + wet_gain * chorus_inout[i];
        }
    }
}

void ensemble_chorus_get_current_modulation(chorus_t *ec, float slow[6], float fast[6])
{
    unsigned channels = ec->channels_;
    const float *parameter = ec->parameter_.get();

    for (unsigned i = 0; i < 6; ++i) {
        slow[i] = 0.5;
        fast[i] = 0.5;
    }
    if (channels == 0)
        return;

    Mono_Chorus &chorus = ec->chorus_[0];

    unsigned lfo_index = 0;
    for (unsigned i = 0; i < 6; ++i) {
        unsigned off = ECP_ENABLE2 - ECP_ENABLE1;
        bool enable = (bool)parameter[ECP_ENABLE1 + i * off];
        if (enable) {
            slow[i] = chorus.current_slow_modulation(lfo_index);
            fast[i] = chorus.current_fast_modulation(lfo_index);
            ++lfo_index;
        }
    }
}

void ensemble_chorus_set_parameter(chorus_t *ec, ec_parameter p, float value)
{
    unsigned channels = ec->channels_;
    Mono_Chorus *chorus = ec->chorus_.get();
    float *parameter = ec->parameter_.get();
    bool must_update_delays = false;
    bool must_update_lfo_depths = false;

    switch (p) {
    case ECP_BYPASS:
        value = value ? 1.0f : 0.0f;
        break;
    case ECP_DELAY:
        value = jsl::clamp(value, 0.0f, 1.0f);
        for (unsigned c = 0; c < channels; ++c)
            chorus[c].delay(value);
        break;
    case ECP_NSTAGES: {
        unsigned nstages = BBD_Line::adjust_nstages(value);
        value = nstages;
        for (unsigned c = 0; c < channels; ++c)
            chorus[c].nstages(nstages);
        break;
    }
    case ECP_MOD_RANGE: {
        value = jsl::clamp(value, 0.0f, 1.0f);
        for (unsigned c = 0; c < channels; ++c)
            chorus[c].mod_range(value);
        break;
    }
    case ECP_SLOW_RATE:
        value = jsl::clamp(value, 0.0f, 1.0f);
        for (unsigned c = 0; c < channels; ++c)
            chorus[c].slow_rate(value);
        break;
    case ECP_SLOW_RAND: {
        value = jsl::clamp(value, 0.0f, 1.0f);
        for (unsigned c = 0; c < channels; ++c)
            chorus[c].slow_rand(value);
        break;
    }
    case ECP_SLOW_WAVE: {
        int wave = jsl::clamp((int)value, 0, (int)Mono_Chorus::lfo_wave_count - 1);
        value = wave;
        for (unsigned c = 0; c < channels; ++c)
            chorus[c].slow_wave(wave);
        break;
    }
    case ECP_FAST_RATE:
        value = jsl::clamp(value, 0.0f, 1.0f);
        for (unsigned c = 0; c < channels; ++c)
            chorus[c].fast_rate(value);
        break;
    case ECP_FAST_WAVE: {
        int wave = jsl::clamp((int)value, 0, (int)Mono_Chorus::lfo_wave_count - 1);
        value = wave;
        for (unsigned c = 0; c < channels; ++c)
            chorus[c].fast_wave(wave);
        break;
    }
    case ECP_FAST_RAND: {
        value = jsl::clamp(value, 0.0f, 1.0f);
        for (unsigned c = 0; c < channels; ++c)
            chorus[c].fast_rand(value);
        break;
    }
    case ECP_LPF_CUTOFF: {
        value = jsl::clamp(value, 0.0f, 1.0f);
        for (unsigned c = 0; c < channels; ++c)
            chorus[c].lpf(value, parameter[ECP_LPF_Q]);
        break;
    }
    case ECP_LPF_Q: {
        value = jsl::clamp(value, 0.0f, 1.0f);
        for (unsigned c = 0; c < channels; ++c)
            chorus[c].lpf(parameter[ECP_LPF_CUTOFF], value);
        break;
    }
    case ECP_GAIN_IN:
        value = jsl::clamp(value, 0.0f, 3.0f);
        break;
    case ECP_GAIN_OUT:
        value = jsl::clamp(value, 0.0f, 3.0f);
        break;
    case ECP_MIX_DRY:
        value = jsl::clamp(value, 0.0f, 1.0f);
        break;
    case ECP_MIX_WET:
        value = jsl::clamp(value, 0.0f, 1.0f);
        break;
    case ECP_ENABLE1:
    case ECP_ENABLE2:
    case ECP_ENABLE3:
    case ECP_ENABLE4:
    case ECP_ENABLE5:
    case ECP_ENABLE6:
        value = value ? 1.0f : 0.0f;
        must_update_delays = true;
        break;
    case ECP_PHASE1:
    case ECP_PHASE2:
    case ECP_PHASE3:
    case ECP_PHASE4:
    case ECP_PHASE5:
    case ECP_PHASE6:
        value = value - (int)value;
        value = (value < 0) ? (value + 1) : value;
        must_update_delays = true;
        break;
    case ECP_DEPTH1:
    case ECP_DEPTH2:
    case ECP_DEPTH3:
    case ECP_DEPTH4:
    case ECP_DEPTH5:
    case ECP_DEPTH6: {
        value = jsl::clamp(value, 0.0f, 1.0f);
        must_update_lfo_depths = true;
        break;
    }
    default:
        return;
    }

    parameter[p] = value;

    if (must_update_delays) {
        float phases[6];
        unsigned nphases = 0;
        for (unsigned i = 0; i < 6; ++i) {
            unsigned off = ECP_ENABLE2 - ECP_ENABLE1;
            bool enable = parameter[ECP_ENABLE1 + i * off];
            float phase = parameter[ECP_PHASE1 + i * off];
            if (enable)
                phases[nphases++] = phase;
        }

        for (unsigned c = 0; c < channels; ++c)
            chorus[c].delays(phases, nphases);

        must_update_lfo_depths = true;
    }

    if (must_update_lfo_depths) {
        unsigned nphases = 0;
        for (unsigned i = 0; i < 6; ++i) {
            unsigned off = ECP_ENABLE2 - ECP_ENABLE1;
            bool enable = parameter[ECP_ENABLE1 + i * off];
            float depth = parameter[ECP_DEPTH1 + i * off];
            if (enable) {
                for (unsigned c = 0; c < channels; ++c)
                    chorus[c].modulation_depth(nphases, depth);
                ++nphases;
            }
        }
    }
}

float ensemble_chorus_get_parameter(chorus_t *ec, ec_parameter p)
{
    if (p >= EC_PARAMETER_COUNT)
        return 0;
    return ec->parameter_[p];
}
