//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ensemble_chorus.h"
#include "chorus.h"
#include "bbd.h"
#include "lfos.h"
#include <jsl/math>
#include <memory>
#include <cstring>
#include <cassert>

struct chorus {
    float samplerate_ = 0;
    unsigned bufsize_ = 0;
    std::unique_ptr<Chorus> chorus_;
    std::unique_ptr<float[]> parameter_;
    std::unique_ptr<float[]> tmpbuf_;
};

static void ensemble_chorus_set_parameter_ex(chorus_t *ec, ec_parameter_t p, float value, bool force);

chorus_t *ensemble_chorus_alloc()
{
    chorus *ec = new chorus;
    return ec;
}

void ensemble_chorus_free(chorus_t *ec)
{
    delete ec;
}

bool ensemble_chorus_init(chorus_t *ec, float samplerate, unsigned bufsize)
{
    // don't trust buffer size and take it as nominal value only
    unsigned ref_bufsize = std::min(bufsize, 16u * 1024);
    for (bufsize = 1024; bufsize < ref_bufsize; bufsize <<= 1);

    ec->samplerate_ = samplerate;
    ec->bufsize_ = bufsize;

    Chorus *chorus = new Chorus;
    ec->chorus_.reset(chorus);
    chorus->setup(samplerate, bufsize);

    float *parameters = new float[EC_PARAMETER_COUNT];
    ec->parameter_.reset(parameters);
    for (unsigned i = 0; i < EC_PARAMETER_COUNT; ++i)
        parameters[i] = ensemble_chorus_parameter_default((ec_parameter)i);

    for (unsigned i = 0; i < EC_PARAMETER_COUNT; ++i) {
        bool force = true;
        ensemble_chorus_set_parameter_ex(ec, (ec_parameter)i, parameters[i], force);
    }

    ec->tmpbuf_.reset(new float[2 * bufsize]);
    return true;
}

static void ensemble_chorus_do_process(chorus_t *ec, float *inout[], unsigned nframes)
{
    unsigned bufsize = ec->bufsize_;
    Chorus &chorus = *ec->chorus_;
    const float *parameters = ec->parameter_.get();

    float *tmp = ec->tmpbuf_.get();
    float *chorus_inouts[2] = { tmp, tmp + bufsize };

    bool bypass = parameters[ECP_BYPASS];
    if (bypass)
        return;

    float input_gain = parameters[ECP_GAIN_IN];
    for (unsigned c = 0; c < 2; ++c) {
        float *channel_inout = inout[c];
        for (unsigned i = 0; i < nframes; ++i)
            channel_inout[i] *= input_gain;
    }

    float output_gain = parameters[ECP_GAIN_OUT];
    float dry_gain = parameters[ECP_MIX_DRY] * output_gain;
    float wet_gain = parameters[ECP_MIX_WET] * output_gain;
    ec_channel_layout ecc = (ec_channel_layout)parameters[ECP_CHANNEL_LAYOUT];

    for (unsigned c = 0; c < 2; ++c) {
        float *channel_inout = inout[c];
        std::memcpy(chorus_inouts[c], channel_inout, nframes * sizeof(float));
    }

    unsigned line_routing[6] = {};
    enum { L = 1, R = 2 };
    for (unsigned i = 0; i < 6; ++i) {
        unsigned off = ECP_ENABLE2 - ECP_ENABLE1;
        line_routing[i] |= parameters[ECP_ROUTE_L1 + i * off] ? L : 0;
        line_routing[i] |= parameters[ECP_ROUTE_R1 + i * off] ? R : 0;
    }

    chorus.process(chorus_inouts, nframes, ecc, line_routing);

    for (unsigned c = 0; c < 2; ++c) {
        float *channel_inout = inout[c];
        float *chorus_inout = chorus_inouts[c];
        for (unsigned i = 0; i < nframes; ++i)
            channel_inout[i] = dry_gain * channel_inout[i] + wet_gain * chorus_inout[i];
    }
}

void ensemble_chorus_process(chorus_t *ec, float *inout[], unsigned nframes)
{
    unsigned bufsize = ec->bufsize_;
    unsigned index = 0;

    while (index < nframes) {
        unsigned curr = std::min(nframes - index, bufsize);
        ensemble_chorus_do_process(ec, inout + index, curr);
        index += curr;
    }
}

void ensemble_chorus_get_current_modulation(chorus_t *ec, float slow[6], float fast[6])
{
    const float *parameter = ec->parameter_.get();

    for (unsigned i = 0; i < 6; ++i) {
        slow[i] = 0.5;
        fast[i] = 0.5;
    }

    Chorus &chorus = *ec->chorus_;

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

static void ensemble_chorus_set_parameter_ex(chorus_t *ec, ec_parameter_t p, float value, bool force)
{
    Chorus &chorus = *ec->chorus_;
    float *parameter = ec->parameter_.get();
    bool must_update_delays = false;
    bool must_update_lfo_depths = false;

    value = ensemble_chorus_adjust_parameter(p, value);

    if (force || parameter[p] != value) {
        parameter[p] = value;
        switch (p) {
        case ECP_DELAY:
            chorus.delay(value);
            break;
        case ECP_NSTAGES:
            chorus.nstages(EC_NSTAGES_MIN << (unsigned)value);
            break;
        case ECP_AA_CUTOFF:
            chorus.aa_cutoff(value);
            break;
        case ECP_MOD_RANGE:
            chorus.mod_range(value);
            break;
        case ECP_SLOW_RATE:
            chorus.slow_rate(value);
            break;
        case ECP_SLOW_RAND:
            chorus.slow_rand(value);
            break;
        case ECP_SLOW_WAVE:
            chorus.slow_wave(value);
            break;
        case ECP_FAST_RATE:
            chorus.fast_rate(value);
            break;
        case ECP_FAST_WAVE:
            chorus.fast_wave(value);
            break;
        case ECP_FAST_RAND:
            chorus.fast_rand(value);
            break;
        case ECP_LPF_CUTOFF:
            chorus.lpf(value, parameter[ECP_LPF_Q]);
            break;
        case ECP_LPF_Q:
            chorus.lpf(parameter[ECP_LPF_CUTOFF], value);
            break;
        case ECP_ENABLE1: case ECP_ENABLE2: case ECP_ENABLE3:
        case ECP_ENABLE4: case ECP_ENABLE5: case ECP_ENABLE6:
        case ECP_PHASE1: case ECP_PHASE2: case ECP_PHASE3:
        case ECP_PHASE4: case ECP_PHASE5: case ECP_PHASE6:
            must_update_delays = true;
            break;
        case ECP_DEPTH1: case ECP_DEPTH2: case ECP_DEPTH3:
        case ECP_DEPTH4: case ECP_DEPTH5: case ECP_DEPTH6:
            must_update_lfo_depths = true;
            break;
        default:
            return;
        }
    }

    if (must_update_delays) {
        float phases[6];
        unsigned nums[6];
        unsigned nphases = 0;
        for (unsigned i = 0; i < 6; ++i) {
            unsigned off = ECP_ENABLE2 - ECP_ENABLE1;
            bool enable = parameter[ECP_ENABLE1 + i * off];
            float phase = parameter[ECP_PHASE1 + i * off];
            if (enable) {
                phases[nphases] = phase;
                nums[nphases] = i;
                ++nphases;
            }
        }
        chorus.delays(phases, nums, nphases);
        must_update_lfo_depths = true;
    }

    if (must_update_lfo_depths) {
        unsigned nphases = 0;
        for (unsigned i = 0; i < 6; ++i) {
            unsigned off = ECP_ENABLE2 - ECP_ENABLE1;
            bool enable = parameter[ECP_ENABLE1 + i * off];
            float depth = parameter[ECP_DEPTH1 + i * off];
            if (enable) {
                chorus.modulation_depth(nphases, depth);
                ++nphases;
            }
        }
    }
}

void ensemble_chorus_set_parameter(chorus_t *ec, ec_parameter_t p, float value)
{
    bool force = false;
    ensemble_chorus_set_parameter_ex(ec, p, value, force);
}

float ensemble_chorus_get_parameter(chorus_t *ec, ec_parameter_t p)
{
    if (p >= EC_PARAMETER_COUNT)
        return 0;
    return ec->parameter_[p];
}

float ensemble_chorus_adjust_parameter(ec_parameter_t p, float value)
{
    unsigned flags = ensemble_chorus_parameter_flags(p);

    switch (p) {
    default:
        if (flags & ECP_BOOLEAN)
            value = value ? 1.0f : 0.0f;
        else {
            float min = ensemble_chorus_parameter_min(p);
            float max = ensemble_chorus_parameter_max(p);
            value = jsl::clamp(value, min, max);
            if (flags & ECP_INTEGER)
                value = std::round(value);
        }
        break;
    case ECP_PHASE1: case ECP_PHASE2: case ECP_PHASE3:
    case ECP_PHASE4: case ECP_PHASE5: case ECP_PHASE6:
        value = value - (int)value;
        value = (value < 0) ? (value + 1) : value;
        break;
    }

    return value;
}

unsigned ensemble_chorus_parameter_count()
{
    return EC_PARAMETER_COUNT;
}

const char *ensemble_chorus_parameter_name(ec_parameter_t p)
{
    switch (p) {
    #define EACH(p, min, max, def, flags, label) case ECP_##p: return #p;
    EC_EACH_PARAMETER(EACH)
    #undef EACH
    default:
        return nullptr;
    }
}

ec_parameter_t ensemble_chorus_parameter_by_name(const char *name)
{
    for (unsigned i = 0; i < EC_PARAMETER_COUNT; ++i)
        if (!std::strcmp(name, ensemble_chorus_parameter_name((ec_parameter)i)))
            return (ec_parameter)i;
    return (ec_parameter)-1;
}

float ensemble_chorus_parameter_min(ec_parameter_t p)
{
    switch (p) {
    #define EACH(p, min, max, def, flags, label) case ECP_##p: return min;
    EC_EACH_PARAMETER(EACH)
    #undef EACH
    default:
        return 0;
    }
}

float ensemble_chorus_parameter_max(ec_parameter_t p)
{
    switch (p) {
    #define EACH(p, min, max, def, flags, label) case ECP_##p: return max;
    EC_EACH_PARAMETER(EACH)
    #undef EACH
    default:
        return 0;
    }
}

float ensemble_chorus_parameter_default(ec_parameter_t p)
{
    switch (p) {
    #define EACH(p, min, max, def, flags, label) case ECP_##p: return def;
    EC_EACH_PARAMETER(EACH)
    #undef EACH
    default:
        return 0;
    }
}

unsigned ensemble_chorus_parameter_flags(ec_parameter_t p)
{
    switch (p) {
    #define EACH(p, min, max, def, flags, label) case ECP_##p: return flags;
    EC_EACH_PARAMETER(EACH)
    #undef EACH
    default:
        return 0;
    }
}

const char *ensemble_chorus_parameter_label(ec_parameter_t p)
{
    switch (p) {
    #define EACH(p, min, max, def, flags, label) case ECP_##p: return label;
    EC_EACH_PARAMETER(EACH)
    #undef EACH
    default:
        return nullptr;
    }
}

const char *const *ensemble_chorus_parameter_choices(ec_parameter_t p)
{
    switch (p) {
    case ECP_CHANNEL_LAYOUT: {
        static const char *const choices[] = {
            "Stereo", "Mono"
        };
        return choices;
    }
    case ECP_NSTAGES:
        static const char *const *choices = nullptr;
        if (choices)
            return choices;
        static char buffers[16 * EC_SUPPORTED_NSTAGES_COUNT];
        static const char *tab_choices[EC_SUPPORTED_NSTAGES_COUNT];
        for (unsigned i = 0; i < EC_SUPPORTED_NSTAGES_COUNT; ++i) {
            sprintf(&buffers[16 * i], "%u", EC_NSTAGES_MIN << i);
            tab_choices[i] = &buffers[16 * i];
        }
        choices = tab_choices;
        return choices;
    case ECP_SLOW_WAVE:
    case ECP_FAST_WAVE: {
        static const char *const choices[] = {
            "Sine", "Square", "Saw", "Ramp", "Triangle"
        };
        return choices;
    }
    default:
        return nullptr;
    }
}
