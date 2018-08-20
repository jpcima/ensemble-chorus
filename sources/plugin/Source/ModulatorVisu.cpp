//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ModulatorVisu.h"

//==============================================================================
ModulatorVisu::ModulatorVisu()
{
}

ModulatorVisu::~ModulatorVisu()
{
}

void ModulatorVisu::setValues(float slow, float fast)
{
    if (slow == slow_ && fast == fast_)
        return;

    slow_ = slow;
    fast_ = fast;
    repaint();
}

void ModulatorVisu::paint(Graphics &g)
{
    int w = getWidth();
    int h = getHeight();

    g.setColour(findColour(Slider::textBoxOutlineColourId));
    g.drawRect(0, 0, w, h, 1);

    float r1 = 3.0f;
    float r2 = 2.0f;
    float xpad = 8.0f;
    float x1 = xpad + (w - 2 * xpad) * slow_;
    float x2 = xpad + (w - 2 * xpad) * fast_;
    float y1 = h * (1.0f / 3.0f);
    float y2 = h * (2.0f / 3.0f);

    g.setColour(Colours::black);
    Rectangle<float> rect1;
    rect1.setSize(r1, r1);
    rect1.setCentre(x1, y1);
    g.drawEllipse(rect1, 1.0);
    Rectangle<float> rect2;
    rect2.setSize(r2, r2);
    rect2.setCentre(x2, y2);
    g.drawEllipse(rect2, 1.0);
}
