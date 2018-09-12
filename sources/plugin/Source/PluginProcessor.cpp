//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "PluginProcessor.h"
#if !JUCE_AUDIOPROCESSOR_NO_GUI
#include "PluginEditor.h"
#endif
#include "MessageQueue.h"
#include "utility/stdc_memory.h"
#include <cstdlib>
#include <cassert>

//==============================================================================
EnsembleChorusAudioProcessor::EnsembleChorusAudioProcessor()
{
    setupParameters();
}

EnsembleChorusAudioProcessor::~EnsembleChorusAudioProcessor()
{
}

//==============================================================================
const String EnsembleChorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EnsembleChorusAudioProcessor::acceptsMidi() const
{
    return false;
}

bool EnsembleChorusAudioProcessor::producesMidi() const
{
    return false;
}

bool EnsembleChorusAudioProcessor::isMidiEffect() const
{
    return false;
}

double EnsembleChorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EnsembleChorusAudioProcessor::getNumPrograms()
{
    return 1;
}

int EnsembleChorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EnsembleChorusAudioProcessor::setCurrentProgram(int index)
{
}

const String EnsembleChorusAudioProcessor::getProgramName(int index)
{
    return {};
}

void EnsembleChorusAudioProcessor::changeProgramName(int index, const String &newName)
{
}

//==============================================================================
void EnsembleChorusAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    chorus *ec = ensemble_chorus_alloc();
    ec_.reset(ec);
    ensemble_chorus_init(ec, sampleRate, samplesPerBlock);
    mq_.reset(new MessageQueue(1024));
}

void EnsembleChorusAudioProcessor::releaseResources()
{
    mq_.reset();
    ec_.reset();
}

void EnsembleChorusAudioProcessor::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages)
{
    ScopedNoDenormals noDenormals;

    chorus &ec = *ec_;

    for (unsigned i = 0, n = ensemble_chorus_parameter_count(); i < n; ++i) {
        ec_parameter p = (ec_parameter)i;
        ensemble_chorus_set_parameter(&ec, p, getEcp(p));
    }

    ensemble_chorus_process(&ec, buffer.getArrayOfWritePointers(), buffer.getNumSamples());

    Messages::NotifyModulation msg_mod;
    ensemble_chorus_get_current_modulation(&ec, msg_mod.slow, msg_mod.fast);
    mq_->writeMessage(msg_mod);
}

#if ! JUCE_AUDIOPROCESSOR_NO_GUI
//==============================================================================
bool EnsembleChorusAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor *EnsembleChorusAudioProcessor::createEditor()
{
    return new EnsembleChorusAudioProcessorEditor(*this);
}
#endif

//==============================================================================
float EnsembleChorusAudioProcessor::getEcp(ec_parameter p) const
{
    const AudioProcessorParameter *ap = getParameters()[(unsigned)p];
    unsigned flags = ensemble_chorus_parameter_flags(p);
    switch (flags & (ECP_FLOAT|ECP_BOOLEAN|ECP_INTEGER)) {
    case ECP_BOOLEAN:
        return *static_cast<const AudioParameterBool *>(ap); break;
    case ECP_INTEGER:
        return *static_cast<const AudioParameterInt *>(ap); break;
    case ECP_FLOAT:
        return *static_cast<const AudioParameterFloat *>(ap); break;
    default:
        assert(false); std::abort();
    }
}

void EnsembleChorusAudioProcessor::setEcp(ec_parameter p, float value)
{
    AudioProcessorParameter *ap = getParameters()[(unsigned)p];
    unsigned flags = ensemble_chorus_parameter_flags(p);
    switch (flags & (ECP_FLOAT|ECP_BOOLEAN|ECP_INTEGER)) {
    case ECP_BOOLEAN:
        *static_cast<AudioParameterBool *>(ap) = value; break;
    case ECP_INTEGER:
        *static_cast<AudioParameterInt *>(ap) = value; break;
    case ECP_FLOAT:
        *static_cast<AudioParameterFloat *>(ap) = value; break;
    default:
        assert(false);
    }
}

//==============================================================================
void EnsembleChorusAudioProcessor::getStateInformation(MemoryBlock &destData)
{
    unsigned count = ensemble_chorus_parameter_count();
    std::unique_ptr<float[]> values(new float[count]);

    for (unsigned i = 0; i < count; ++i)
        values[i] = getEcp((ec_parameter)i);

    size_t length = 0;
    stdc_unique_ptr<char> data(ensemble_chorus_save_parameters(&length, values.get()));
    destData.append(data.get(), length);
}

void EnsembleChorusAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    unsigned count = ensemble_chorus_parameter_count();
    std::unique_ptr<float[]> values(new float[count]);

    if (!ensemble_chorus_load_parameters((const char *)data, sizeInBytes, values.get()))
        return;

    for (unsigned i = 0; i < count; ++i)
        setEcp((ec_parameter)i, values[i]);
}

//==============================================================================
void EnsembleChorusAudioProcessor::setupParameters()
{
    unsigned count = ensemble_chorus_parameter_count();

    for (unsigned i = 0; i < count; ++i) {
        ec_parameter p = (ec_parameter)i;
        const char *name = ensemble_chorus_parameter_name(p);
        const char *label = ensemble_chorus_parameter_label(p);
        unsigned flags = ensemble_chorus_parameter_flags(p);
        float min = ensemble_chorus_parameter_min(p);
        float max = ensemble_chorus_parameter_max(p);
        float def = ensemble_chorus_parameter_default(p);

        AudioProcessorParameter *ap = nullptr;
        switch (flags & (ECP_FLOAT|ECP_BOOLEAN|ECP_INTEGER)) {
        case ECP_BOOLEAN:
            ap = new AudioParameterBool(name, label, def); break;
        case ECP_INTEGER:
            ap = new AudioParameterInt(name, label, min, max, def); break;
        case ECP_FLOAT:
            ap = new AudioParameterFloat(name, label, min, max, def); break;
        }
        addParameter(ap);
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new EnsembleChorusAudioProcessor();
}
