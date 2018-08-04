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
EC_API bool ensemble_chorus_init(chorus_t *ec, float samplerate, unsigned bufsize, unsigned channels);
EC_API void ensemble_chorus_process(chorus_t *ec, float *inout[], unsigned nframes);

EC_API void ensemble_chorus_get_current_modulation(chorus_t *ec, float slow[6], float fast[6]);

typedef enum ec_parameter {
    ECP_BYPASS,
    ECP_DELAY,
    ECP_NSTAGES,
    ECP_MOD_RANGE,
    ECP_SLOW_RATE,
    ECP_SLOW_WAVE,
    ECP_SLOW_RAND,
    ECP_FAST_RATE,
    ECP_FAST_WAVE,
    ECP_FAST_RAND,
    ECP_LPF_CUTOFF,
    ECP_LPF_Q,
    ECP_GAIN_IN,
    ECP_GAIN_OUT,
    ECP_MIX_DRY,
    ECP_MIX_WET,

    ECP_ENABLE1,
    ECP_PHASE1,
    ECP_DEPTH1,

    ECP_ENABLE2,
    ECP_PHASE2,
    ECP_DEPTH2,

    ECP_ENABLE3,
    ECP_PHASE3,
    ECP_DEPTH3,

    ECP_ENABLE4,
    ECP_PHASE4,
    ECP_DEPTH4,

    ECP_ENABLE5,
    ECP_PHASE5,
    ECP_DEPTH5,

    ECP_ENABLE6,
    ECP_PHASE6,
    ECP_DEPTH6,

    EC_PARAMETER_COUNT
} ec_parameter_t;

EC_API void ensemble_chorus_set_parameter(chorus_t *ec, ec_parameter p, float value);
EC_API float ensemble_chorus_get_parameter(chorus_t *ec, ec_parameter p);

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
