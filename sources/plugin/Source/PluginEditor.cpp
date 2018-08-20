//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MainComponent.h"
#include "LookAndFeel.h"
#include "MessageQueue.h"
#include "utility/FunctionalTimer.h"

//==============================================================================
EnsembleChorusAudioProcessorEditor::EnsembleChorusAudioProcessorEditor(EnsembleChorusAudioProcessor &p)
    : AudioProcessorEditor(&p)
    , processor_(p)
{
    MyLookAndFeel *lnf = new MyLookAndFeel;
    lnf_.reset(lnf);
    LookAndFeel::setDefaultLookAndFeel(lnf);

    MainComponent *component = new MainComponent(p);
    component_.reset(component);

    addAndMakeVisible(component);
    setSize(component->getWidth(), component->getHeight());

    for (AudioProcessorParameter *ap : p.getParameters())
        ap->addListener(this);

    Timer *mq_timer = FunctionalTimer::create([this]{ handleUpdates(); });
    mq_timer_.reset(mq_timer);
    mq_timer->startTimer(30);
}

EnsembleChorusAudioProcessorEditor::~EnsembleChorusAudioProcessorEditor()
{
    EnsembleChorusAudioProcessor &processor = processor_;

    for (AudioProcessorParameter *ap : processor.getParameters())
        ap->removeListener(this);
}

void EnsembleChorusAudioProcessorEditor::parameterValueChanged(int parameterIndex, float newValue)
{
    parameter_changed_ = true;
}

void EnsembleChorusAudioProcessorEditor::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
}

void EnsembleChorusAudioProcessorEditor::handleUpdates()
{
    EnsembleChorusAudioProcessor &processor = processor_;
    MainComponent &component = *component_;

    if (std::shared_ptr<MessageQueue> mq = processor.getMessageQueue()) {
        std::unique_ptr<uint8_t[]> buf(Messages::allocateBuffer());
        auto &msg = *reinterpret_cast<BasicMessage *>(buf.get());

        while (mq->readMessage(msg)) {
            switch (msg.tag) {
            case MessageTag::NotifyModulation: {
                const auto &body = static_cast<Messages::NotifyModulation &>(msg);
                component.setModulationValues(body.slow, body.fast);
                break;
            }
            }
        }
    }

    if (parameter_changed_.compareAndSetBool(false, true)) {
        for (unsigned i = 0, n = ensemble_chorus_parameter_count(); i < n; ++i) {
            ec_parameter p = (ec_parameter)i;
            component.updateDisplayWithEcp(p, processor.getEcp(p));
        }
    }
}
