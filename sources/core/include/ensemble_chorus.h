//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// The Ensemble Chorus Library - Chorus effect with modulated BBD delay lines //

#pragma once
#include <math.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(EC_API) && defined(_WIN32)
#   define EC_API __declspec(dllimport)
#elif !defined(EC_API)
#   define EC_API
#endif

typedef struct chorus chorus_t;

EC_API chorus_t *ensemble_chorus_alloc();
EC_API void ensemble_chorus_free(chorus_t *ec);
EC_API bool ensemble_chorus_init(chorus_t *ec, float samplerate, unsigned bufsize);
EC_API void ensemble_chorus_process(chorus_t *ec, float *inout[], unsigned nframes);

EC_API void ensemble_chorus_get_current_modulation(chorus_t *ec, float slow[6], float fast[6]);

typedef enum ec_channel_layout {
    ECC_STEREO,
    ECC_MONO
} ec_channel_layout_t;

enum {
    EC_SUPPORTED_NSTAGES_COUNT = 5,
    EC_NSTAGES_MIN = 512,
    EC_NSTAGES_MAX = EC_NSTAGES_MIN << (EC_SUPPORTED_NSTAGES_COUNT - 1),
    EC_LFO_WAVE_COUNT = 5,
    EC_CLOCK_RATE_MIN = 1500,
    EC_DELAY_DEFAULT_MS = 50
};

typedef enum ec_parameter_flag {
    ECP_FLOAT       = 0,
    ECP_BOOLEAN     = 1,
    ECP_INTEGER     = 2,
    ECP_CHOICE      = 4,
    ECP_LOGARITHMIC = 8
} ec_parameter_flag_t;

#define EC_EACH_PARAMETER(P)                                            \
    /* Name, Min, Max, Def, Flags, Label */                             \
    P(BYPASS, false, true, false, ECP_BOOLEAN, "Bypass")                \
    P(CHANNEL_LAYOUT, ECC_STEREO, ECC_MONO, ECC_STEREO, ECP_INTEGER|ECP_CHOICE, "Channel layout") \
    P(DELAY, 0, 3.0, 1e-3 * EC_DELAY_DEFAULT_MS, ECP_FLOAT, "Delay")    \
    P(NSTAGES, 0, EC_SUPPORTED_NSTAGES_COUNT - 1, 1/* 1024 */, ECP_INTEGER|ECP_CHOICE, "Stages") \
    P(AA_CUTOFF, 0, 22000, 15000, ECP_FLOAT, "Anti-alias cutoff")       \
    P(MOD_RANGE, 0, 1, 0.5, ECP_FLOAT, "Modulation range")              \
    P(SLOW_RATE, 0, 1, 0.5, ECP_FLOAT, "Chorus rate")                   \
    P(SLOW_WAVE, 0, EC_LFO_WAVE_COUNT - 1, 4, ECP_INTEGER|ECP_CHOICE, "Chorus wave") \
    P(SLOW_RAND, 0, 1, 0.1, ECP_FLOAT, "Chorus random")                 \
    P(FAST_RATE, 0, 1, 0.5, ECP_FLOAT, "Vibrato rate")                  \
    P(FAST_WAVE, 0, EC_LFO_WAVE_COUNT - 1, 4, ECP_INTEGER|ECP_CHOICE, "Vibrato wave") \
    P(FAST_RAND, 0, 1, 0.1, ECP_FLOAT, "Vibrato random")                \
    P(LPF_CUTOFF, 0, 22000, 22000, ECP_FLOAT, "Lowpass cutoff")         \
    P(LPF_Q, 0, 1, M_SQRT1_2, ECP_FLOAT, "Lowpass Q")                   \
    P(GAIN_IN, 0, 3, 1, ECP_FLOAT|ECP_LOGARITHMIC, "Input gain")        \
    P(GAIN_OUT, 0, 3, 1, ECP_FLOAT|ECP_LOGARITHMIC, "Output gain")      \
    P(MIX_DRY, 0, 1, M_SQRT1_2, ECP_FLOAT|ECP_LOGARITHMIC, "Dry mix")   \
    P(MIX_WET, 0, 1, M_SQRT1_2, ECP_FLOAT|ECP_LOGARITHMIC, "Wet mix")   \
    EC_EACH_LINE_PARAMETER(P, 1)                                        \
    EC_EACH_LINE_PARAMETER(P, 2)                                        \
    EC_EACH_LINE_PARAMETER(P, 3)                                        \
    EC_EACH_LINE_PARAMETER(P, 4)                                        \
    EC_EACH_LINE_PARAMETER(P, 5)                                        \
    EC_EACH_LINE_PARAMETER(P, 6)

#define EC_EACH_LINE_PARAMETER(P, I)                                    \
    /* Name, Min, Max, Def, Flags, Label */                             \
    P(ENABLE##I, false, true, (I <= 3), ECP_BOOLEAN, "Enable " #I)      \
    P(PHASE##I, 0, 360, (((I - 1) % 3) * 120 + ((I - 1) / 3) * 60), ECP_FLOAT, "Phase " #I) \
    P(DEPTH##I, 0, 1, 0.5, ECP_FLOAT, "Depth " #I)                      \
    P(ROUTE_L##I, false, true, (I != 3 && I != 4), ECP_BOOLEAN, "Left " #I) \
    P(ROUTE_R##I, false, true, (I != 1 && I != 6), ECP_BOOLEAN, "Right " #I)

typedef enum ec_parameter {
    #define EACH(p, min, max, def, flags, label) ECP_##p,
    EC_EACH_PARAMETER(EACH)
    #undef EACH
    EC_PARAMETER_COUNT
} ec_parameter_t;

EC_API void ensemble_chorus_set_parameter(chorus_t *ec, ec_parameter_t p, float value);
EC_API float ensemble_chorus_get_parameter(chorus_t *ec, ec_parameter_t p);
EC_API float ensemble_chorus_adjust_parameter(ec_parameter_t p, float value);

EC_API unsigned ensemble_chorus_parameter_count();
EC_API const char *ensemble_chorus_parameter_name(ec_parameter_t p);
EC_API ec_parameter_t ensemble_chorus_parameter_by_name(const char *name);
EC_API float ensemble_chorus_parameter_min(ec_parameter_t p);
EC_API float ensemble_chorus_parameter_max(ec_parameter_t p);
EC_API float ensemble_chorus_parameter_default(ec_parameter_t p);
EC_API unsigned ensemble_chorus_parameter_flags(ec_parameter_t p);
EC_API const char *ensemble_chorus_parameter_label(ec_parameter_t p);
EC_API const char *const *ensemble_chorus_parameter_choices(ec_parameter_t p);

EC_API bool ensemble_chorus_load_parameters(const char *data, size_t length, float *parameters);
EC_API char *ensemble_chorus_save_parameters(size_t *length, const float *parameters);

#if defined(__cplusplus)
}  // extern "C"
#endif

#if defined(__cplusplus)
#include <memory>

struct ec_deleter {
    void operator()(chorus *x) const noexcept
        { ensemble_chorus_free(x); }
};
typedef std::unique_ptr<chorus, ec_deleter> chorus_u;
#endif
