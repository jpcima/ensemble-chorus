#include "PluginEnsembleChorus.hpp"
#include "EnsembleChorusShared.hpp"
#include <DenormalDisabler.h>
#include <utility>
#include <cstring>

using WebCore::DenormalDisabler;

// -----------------------------------------------------------------------

PluginEnsembleChorus::PluginEnsembleChorus()
    : Plugin(ParameterCount, FactoryPresetCount, 0)
{
    chorus_t *ec = ensemble_chorus_alloc();
    fChorus.reset(ec);

    fSampleRate = getSampleRate();
    fBufferSize = std::max<uint32_t>(16, getBufferSize());
    ensemble_chorus_init(ec, fSampleRate, fBufferSize);

    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void PluginEnsembleChorus::initParameter(uint32_t index, Parameter &parameter)
{
    InitParameter(index, parameter);
}

/**
  Set the name of the program @a index.
  This function will be called once, shortly after the plugin is created.
*/
void PluginEnsembleChorus::initProgramName(uint32_t index, String &programName)
{
    DISTRHO_SAFE_ASSERT_RETURN(index < FactoryPresetCount, );

    programName = FactoryPresets[index].name;
}

// -----------------------------------------------------------------------
// Internal data

/**
  Optional callback to inform the plugin about a sample rate change.
*/
void PluginEnsembleChorus::sampleRateChanged(double newSampleRate)
{
    if (fSampleRate == newSampleRate)
        return;

    fSampleRate = newSampleRate;
    updateSampleRateOrBufferSize();
}

void PluginEnsembleChorus::bufferSizeChanged(uint32_t newBufferSize)
{
    newBufferSize = std::max<uint32_t>(16, newBufferSize);

    if (fBufferSize == newBufferSize)
        return;

    fBufferSize = newBufferSize;
    updateSampleRateOrBufferSize();
}

void PluginEnsembleChorus::updateSampleRateOrBufferSize()
{
    chorus_t *ec = fChorus.get();
    float param[EC_PARAMETER_COUNT];

    for (unsigned i = 0; i < EC_PARAMETER_COUNT; ++i)
        param[i] = ensemble_chorus_get_parameter(ec, (ec_parameter_t)i);

    ensemble_chorus_init(ec, fSampleRate, fBufferSize);

    for (unsigned i = 0; i < EC_PARAMETER_COUNT; ++i)
        ensemble_chorus_set_parameter(ec, (ec_parameter_t)i, param[i]);
}

/**
  Get the current value of a parameter.
*/
float PluginEnsembleChorus::getParameterValue(uint32_t index) const
{

    chorus_t *ec = fChorus.get();

    if (index < EC_PARAMETER_COUNT)
        return ensemble_chorus_get_parameter(ec, (ec_parameter_t)index);

    switch (index) {
    case kOutSlowModulator1: case kOutSlowModulator2:
    case kOutSlowModulator3: case kOutSlowModulator4:
    case kOutSlowModulator5: case kOutSlowModulator6:
        return fCurrentSlowModulator[index - kOutSlowModulator1];

    case kOutFastModulator1: case kOutFastModulator2:
    case kOutFastModulator3: case kOutFastModulator4:
    case kOutFastModulator5: case kOutFastModulator6:
        return fCurrentFastModulator[index - kOutFastModulator1];

    default:
        DISTRHO_SAFE_ASSERT_RETURN(false, 0.0f);
    }
}

/**
  Change a parameter value.
*/
void PluginEnsembleChorus::setParameterValue(uint32_t index, float value)
{
    DISTRHO_SAFE_ASSERT_RETURN(index < EC_PARAMETER_COUNT, );

    chorus_t *ec = fChorus.get();
    ensemble_chorus_set_parameter(ec, (ec_parameter_t)index, value);
}

/**
  Load a program.
  The host may call this function from any context,
  including realtime processing.
*/
void PluginEnsembleChorus::loadProgram(uint32_t index)
{
    DISTRHO_SAFE_ASSERT_RETURN(index < FactoryPresetCount, );

    for (int i = 0; i < EC_PARAMETER_COUNT; i++)
        setParameterValue(i, FactoryPresets[index].params[i]);
}

// -----------------------------------------------------------------------
// Process

void PluginEnsembleChorus::run(const float **inputs, float **outputs, uint32_t frames)
{
    DenormalDisabler dd;

    for (unsigned c = 0; c < 2; ++c) {
        if (inputs[c] != outputs[c])
            memcpy(outputs[c], inputs[c], frames * sizeof(float));
    }

    chorus_t *ec = fChorus.get();
    const uint32_t bufferSize = fBufferSize;

    uint32_t index = 0;
    while (index < frames) {
        uint32_t count = std::min(bufferSize, frames - index);
        float *inout[2] = { outputs[0] + index, outputs[1] + index };
        ensemble_chorus_process(ec, inout, frames);
        index += count;
    }

    ensemble_chorus_get_current_modulation(
        ec, fCurrentSlowModulator, fCurrentFastModulator);
}

// -----------------------------------------------------------------------

Plugin *DISTRHO::createPlugin()
{
    return new PluginEnsembleChorus;
}

// -----------------------------------------------------------------------
