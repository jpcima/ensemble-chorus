//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "modulation_view.h"
#include "graphics/box.h"
#include "graphics/colors.h"

Modulation_View::Modulation_View(NanoWidget *groupWidget)
    : NanoWidget(groupWidget)
{
}

void Modulation_View::setValues(float slow, float fast)
{
    if (slow == slow_ && fast == fast_)
        return;

    slow_ = slow;
    fast_ = fast;
    repaint();
}

void Modulation_View::onNanoDisplay()
{
    Box_Style bs = BS_DOWN_BOX;
    float x = getAbsoluteX(), y = getAbsoluteY();
    float w = getWidth(), h = getHeight();

    styled_box(*this, bs, x, y, w, h, Colors::background());

    float dx, dy, dw, dh;
    styled_box_deltas(bs, &dx, &dy, &dw, &dh);

    x += dx;
    y += dy;
    w -= dw;
    h -= dh;

    double r1 = 2.0;
    double r2 = 1.5;
    double xpad = 8.0;
    double x1 = x + xpad + (w - 2 * xpad) * slow_;
    double x2 = x + xpad + (w - 2 * xpad) * fast_;
    double y1 = y + h * (1.0 / 3.0);
    double y2 = y + h * (2.0 / 3.0);

    strokeColor(Colors::gray0());

    beginPath();
    circle(x1, y1, r1);
    stroke();

    beginPath();
    circle(x2, y2, r2);
    stroke();
}
