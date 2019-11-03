#pragma once
#include "DistrhoPlugin.hpp"
#include <ensemble_chorus.h>

enum {
    kOutSlowModulator1 = EC_PARAMETER_COUNT,
    kOutSlowModulator2,
    kOutSlowModulator3,
    kOutSlowModulator4,
    kOutSlowModulator5,
    kOutSlowModulator6,

    kOutFastModulator1,
    kOutFastModulator2,
    kOutFastModulator3,
    kOutFastModulator4,
    kOutFastModulator5,
    kOutFastModulator6,

    ParameterCount
};

struct Preset {
    const char *name;
    float params[EC_PARAMETER_COUNT];
};

extern const Preset FactoryPresets[];
extern const uint FactoryPresetCount;

void InitParameter(uint32_t index, Parameter &parameter);
