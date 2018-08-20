//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"

//==============================================================================
/*
 */
class ModulatorVisu : public Component {
public:
    ModulatorVisu();
    ~ModulatorVisu();

    void setValues(float slow, float fast);
    void paint(Graphics &) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulatorVisu)

    float slow_ = 0.5f;
    float fast_ = 0.5f;
};
