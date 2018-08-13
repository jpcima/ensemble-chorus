//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "plugin/editor/editor.h"

Chorus_UI::Chorus_UI()
    : UI(625, 280)
{
}

void Chorus_UI::parameterChanged(uint32_t index, float value)
{
    // TODO
    
}

void Chorus_UI::onNanoDisplay()
{
    // TODO
    
}

UI *DISTRHO::createUI()
{
    return new Chorus_UI;
}
