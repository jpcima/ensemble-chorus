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
};

typedef enum ec_parameter_flag {
    ECP_FLOAT       = 0,
    ECP_BOOLEAN     = 1,
    ECP_INTEGER     = 2,
    ECP_LOGARITHMIC = 4,
} ec_parameter_flag_t;

#define  EC_EACH_PARAMETER(P)                   \
    /* Name, Min, Max, Def, Flags */            \
    P(BYPASS, false, true, false, ECP_BOOLEAN)  \
    P(CHANNEL_LAYOUT, ECC_STEREO, ECC_MONO, ECC_STEREO, ECP_INTEGER) \
    P(DELAY, 0, 1, 0.5, ECP_FLOAT)              \
    P(NSTAGES, EC_NSTAGES_MIN, EC_NSTAGES_MAX, 1024, ECP_INTEGER) \
    P(MOD_RANGE, 0, 1, 0.5, ECP_FLOAT)          \
    P(SLOW_RATE, 0, 1, 0.5, ECP_FLOAT)          \
    P(SLOW_WAVE, 0, EC_LFO_WAVE_COUNT - 1, 4, ECP_INTEGER) \
    P(SLOW_RAND, 0, 1, 0.1, ECP_FLOAT)          \
    P(FAST_RATE, 0, 1, 0.5, ECP_FLOAT)          \
    P(FAST_WAVE, 0, EC_LFO_WAVE_COUNT - 1, 4, ECP_INTEGER) \
    P(FAST_RAND, 0, 1, 0.1, ECP_FLOAT)          \
    P(LPF_CUTOFF, 0, 1, 1.0, ECP_FLOAT)         \
    P(LPF_Q, 0, 1, M_SQRT1_2, ECP_FLOAT)        \
    P(GAIN_IN, 0, 3, 1, ECP_FLOAT|ECP_LOGARITHMIC) \
    P(GAIN_OUT, 0, 3, 1, ECP_FLOAT|ECP_LOGARITHMIC) \
    P(MIX_DRY, 0, 1, M_SQRT1_2, ECP_FLOAT|ECP_LOGARITHMIC) \
    P(MIX_WET, 0, 1, M_SQRT1_2, ECP_FLOAT|ECP_LOGARITHMIC) \
    /* */                                       \
    P(ENABLE1, false, true, true, ECP_BOOLEAN)  \
    P(PHASE1, 0, 1, 0, ECP_FLOAT)               \
    P(DEPTH1, 0, 1, 0.5, ECP_FLOAT)             \
    P(ROUTE_L1, false, true, true, ECP_BOOLEAN) \
    P(ROUTE_R1, false, true, false, ECP_BOOLEAN) \
    /* */                                       \
    P(ENABLE2, false, true, true, ECP_BOOLEAN)  \
    P(PHASE2, 0, 1, 1. / 3., ECP_FLOAT)         \
    P(DEPTH2, 0, 1, 0.5, ECP_FLOAT)             \
    P(ROUTE_L2, false, true, true, ECP_BOOLEAN) \
    P(ROUTE_R2, false, true, true, ECP_BOOLEAN) \
    /* */                                       \
    P(ENABLE3, false, true, true, ECP_BOOLEAN)  \
    P(PHASE3, 0, 1, 2. / 3., ECP_FLOAT)         \
    P(DEPTH3, 0, 1, 0.5, ECP_FLOAT)             \
    P(ROUTE_L3, false, true, false, ECP_BOOLEAN) \
    P(ROUTE_R3, false, true, true, ECP_BOOLEAN) \
    /* */                                       \
    P(ENABLE4, false, true, false, ECP_BOOLEAN) \
    P(PHASE4, 0, 1, 0, ECP_FLOAT)               \
    P(DEPTH4, 0, 1, 0.5, ECP_FLOAT)             \
    P(ROUTE_L4, false, true, false, ECP_BOOLEAN) \
    P(ROUTE_R4, false, true, true, ECP_BOOLEAN) \
    /* */                                       \
    P(ENABLE5, false, true, false, ECP_BOOLEAN) \
    P(PHASE5, 0, 1, 1. / 3., ECP_FLOAT)         \
    P(DEPTH5, 0, 1, 0.5, ECP_FLOAT)             \
    P(ROUTE_L5, false, true, true, ECP_BOOLEAN) \
    P(ROUTE_R5, false, true, true, ECP_BOOLEAN) \
    /* */                                       \
    P(ENABLE6, false, true, false, ECP_BOOLEAN) \
    P(PHASE6, 0, 1, 2. / 3., ECP_FLOAT)         \
    P(DEPTH6, 0, 1, 0.5, ECP_FLOAT)             \
    P(ROUTE_L6, false, true, true, ECP_BOOLEAN) \
    P(ROUTE_R6, false, true, false, ECP_BOOLEAN)

typedef enum ec_parameter {
    #define EACH(p, min, max, def, flags) ECP_##p,
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
