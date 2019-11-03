#include "EnsembleChorusShared.hpp"

const Preset FactoryPresets[] = {
    {
        "Default",
        {
            #define EACH(Name, Min, Max, Def, Flags, Label) (Def),
            EC_EACH_PARAMETER(EACH)
            #undef EACH
        }
    },
};

const uint FactoryPresetCount = sizeof(FactoryPresets) / sizeof(Preset);

void InitParameter(uint32_t index, Parameter &parameter)
{
    DISTRHO_SAFE_ASSERT_RETURN(index < ParameterCount, );

    parameter.hints |= kParameterIsAutomable;

    switch (index) {
    case ECP_BYPASS:
        parameter.initDesignation(kParameterDesignationBypass);
        return;

    case kOutSlowModulator1: case kOutSlowModulator2:
    case kOutSlowModulator3: case kOutSlowModulator4:
    case kOutSlowModulator5: case kOutSlowModulator6:
    {
        unsigned n = index - kOutSlowModulator1;
        parameter.symbol = "OUT_SLOW_MODULATOR" + String(n + 1);
        parameter.name = "Slow modulator output " + String(n + 1);
        parameter.ranges = ParameterRanges(0.0f, 0.0f, 1.0f);
        parameter.hints |= kParameterIsOutput;
        return;
    }

    case kOutFastModulator1: case kOutFastModulator2:
    case kOutFastModulator3: case kOutFastModulator4:
    case kOutFastModulator5: case kOutFastModulator6:
    {
        unsigned n = index - kOutFastModulator1;
        parameter.symbol = "OUT_FAST_MODULATOR" + String(n + 1);
        parameter.name = "Fast modulator output " + String(n + 1);
        parameter.ranges = ParameterRanges(0.0f, 0.0f, 1.0f);
        parameter.hints |= kParameterIsOutput;
        return;
    }
    }

    ec_parameter_t ecp = (ec_parameter_t)index;

    parameter.symbol = ensemble_chorus_parameter_name(ecp);
    parameter.name = ensemble_chorus_parameter_label(ecp);

    parameter.ranges.min = ensemble_chorus_parameter_min(ecp);
    parameter.ranges.max = ensemble_chorus_parameter_max(ecp);
    parameter.ranges.def = ensemble_chorus_parameter_default(ecp);

    unsigned flags = ensemble_chorus_parameter_flags(ecp);

    if (flags & ECP_BOOLEAN) {
        parameter.hints |= kParameterIsBoolean|kParameterIsInteger;
    }
    else if (flags & ECP_INTEGER) {
        parameter.hints |= kParameterIsInteger;
        if (flags & ECP_CHOICE) {
            const char *const *choices = ensemble_chorus_parameter_choices(ecp);

            const int v1 = (int)parameter.ranges.min;
            const int v2 = (int)parameter.ranges.max;
            const int step = (v1 < v2) ? +1 : -1;
            const unsigned count = ((v1 < v2) ? (v2 - v1) : (v1 - v2)) + 1;

            ParameterEnumerationValue *pev = new ParameterEnumerationValue[count];
            parameter.enumValues.values = pev;
            parameter.enumValues.count = count;

            int value = v1;
            for (unsigned i = 0; i < count; ++i, value += step) {
                pev[i].label = choices[i];
                pev[i].value = value;
            }
        }
    }

    if (flags & ECP_LOGARITHMIC)
        parameter.hints |= kParameterIsLogarithmic;
}
