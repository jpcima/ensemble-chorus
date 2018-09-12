//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"
#include "ensemble_chorus.h"
class MessageQueue;

//==============================================================================
/**
 */
class EnsembleChorusAudioProcessor : public AudioProcessor {
public:
    //==============================================================================
    EnsembleChorusAudioProcessor();
    ~EnsembleChorusAudioProcessor();

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(AudioBuffer<float> &, MidiBuffer &) override;

#if ! JUCE_AUDIOPROCESSOR_NO_GUI
    //==============================================================================
    AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;
#endif

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String &newName) override;

    //==============================================================================
    float getEcp(ec_parameter p) const;
    void setEcp(ec_parameter p, float value);

    //==============================================================================
    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    //==============================================================================
    const std::shared_ptr<MessageQueue> &getMessageQueue() const
        { return mq_; }

private:
    void setupParameters();

    chorus_u ec_;
    std::shared_ptr<MessageQueue> mq_;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnsembleChorusAudioProcessor)
};
