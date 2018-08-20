//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"
#include "PluginProcessor.h"
class MainComponent;
class MyLookAndFeel;

//==============================================================================
/**
 */
class EnsembleChorusAudioProcessorEditor :
    public AudioProcessorEditor,
    public AudioProcessorParameter::Listener {
public:
    EnsembleChorusAudioProcessorEditor(EnsembleChorusAudioProcessor &);
    ~EnsembleChorusAudioProcessorEditor();

protected:
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;

private:
    void handleUpdates();

    EnsembleChorusAudioProcessor &processor_;
    std::unique_ptr<MainComponent> component_;
    std::unique_ptr<MyLookAndFeel> lnf_;

    std::unique_ptr<Timer> mq_timer_;
    Atomic<bool> parameter_changed_ = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnsembleChorusAudioProcessorEditor)
};
