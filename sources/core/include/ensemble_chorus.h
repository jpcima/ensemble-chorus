//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// The Ensemble Chorus Library - Chorus effect with modulated BBD delay lines //

#pragma once

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

#define EC_EACH_PARAMETER(P)                    \
    P(BYPASS)                                   \
    P(CHANNEL_LAYOUT)                           \
    P(DELAY)                                    \
    P(NSTAGES)                                  \
    P(MOD_RANGE)                                \
    P(SLOW_RATE)                                \
    P(SLOW_WAVE)                                \
    P(SLOW_RAND)                                \
    P(FAST_RATE)                                \
    P(FAST_WAVE)                                \
    P(FAST_RAND)                                \
    P(LPF_CUTOFF)                               \
    P(LPF_Q)                                    \
    P(GAIN_IN)                                  \
    P(GAIN_OUT)                                 \
    P(MIX_DRY)                                  \
    P(MIX_WET)                                  \
    /* */                                       \
    P(ENABLE1)                                  \
    P(PHASE1)                                   \
    P(DEPTH1)                                   \
    P(ROUTE_L1)                                 \
    P(ROUTE_R1)                                 \
    /* */                                       \
    P(ENABLE2)                                  \
    P(PHASE2)                                   \
    P(DEPTH2)                                   \
    P(ROUTE_L2)                                 \
    P(ROUTE_R2)                                 \
    /* */                                       \
    P(ENABLE3)                                  \
    P(PHASE3)                                   \
    P(DEPTH3)                                   \
    P(ROUTE_L3)                                 \
    P(ROUTE_R3)                                 \
    /* */                                       \
    P(ENABLE4)                                  \
    P(PHASE4)                                   \
    P(DEPTH4)                                   \
    P(ROUTE_L4)                                 \
    P(ROUTE_R4)                                 \
    /* */                                       \
    P(ENABLE5)                                  \
    P(PHASE5)                                   \
    P(DEPTH5)                                   \
    P(ROUTE_L5)                                 \
    P(ROUTE_R5)                                 \
    /* */                                       \
    P(ENABLE6)                                  \
    P(PHASE6)                                   \
    P(DEPTH6)                                   \
    P(ROUTE_L6)                                 \
    P(ROUTE_R6)

typedef enum ec_parameter {
    #define EACH(p) ECP_##p,
    EC_EACH_PARAMETER(EACH)
    #undef EACH
    EC_PARAMETER_COUNT
} ec_parameter_t;

EC_API void ensemble_chorus_set_parameter(chorus_t *ec, ec_parameter_t p, float value);
EC_API float ensemble_chorus_get_parameter(chorus_t *ec, ec_parameter_t p);

EC_API unsigned ensemble_chorus_parameter_count();
EC_API const char *ensemble_chorus_parameter_name(ec_parameter_t p);
EC_API ec_parameter_t ensemble_chorus_parameter_by_name(const char *name);

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
