//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "ensemble_chorus.h"
#include <distrho/DistrhoPlugin.hpp>
#include <memory>
class Message_Queue;

class Chorus_Plugin : public Plugin {
public:
    Chorus_Plugin();
    ~Chorus_Plugin();
    const char *getLabel() const override;
    const char *getMaker() const override;
    const char *getLicense() const override;
    uint32_t getVersion() const override;
    int64_t getUniqueId() const override;
    void initParameter(uint32_t index, Parameter &parameter) override;
    float getParameterValue(uint32_t index) const override;
    void setParameterValue(uint32_t index, float value) override;
    void activate() override;
    void run(const float **inputs, float **outputs, uint32_t frames) override;
    Message_Queue &messageQueueToUI() { return *mq_to_ui_; }
private:
    chorus_u ec_;
    std::unique_ptr<Message_Queue> mq_to_ui_;
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Chorus_Plugin)
};
