//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "slider.h"
#include <cassert>

Slider::Slider(NanoWidget *groupWidget)
    : NanoWidget(groupWidget)
{
    setSize(50, 50);
}

void Slider::setRange(float min, float max)
{
    assert(min < max);

    float value = value_;
    min_ = min;
    max_ = max;

    if (value < min) {
        value = value_ = min;
        repaint();

        if (callback_)
            callback_->sliderValueChanged(this, value);
    }
    else if (value > max) {
        value = value_ = max;
        repaint();

        if (callback_)
            callback_->sliderValueChanged(this, value);
    }
}

void Slider::setValue(float value, bool sendCallback)
{
    if (value == value_)
        return;

    value_ = value;

    repaint();

    if (sendCallback && callback_)
        callback_->sliderValueChanged(this, value);
}

void Slider::setType(Type t)
{
    if (t == type_)
        return;

    type_ = t;
    repaint();
}

void Slider::onNanoDisplay()
{
    int x = getAbsoluteX();
    int y = getAbsoluteY();
    int w = getWidth();
    int h = getHeight();
    float dx, dy, dw, dh;

    styled_box_deltas(box_, &dx, &dy, &dw, &dh);
    drawInBounds(x + dx, y + dy, w - dw, h - dh);
}

void Slider::drawInBounds(float x, float y, float w, float h)
{
    float value = (value_ - min_) / (max_ - min_);
    if (!isHorizontal())
        value = 1 - value;

    //
    float ww = isHorizontal() ? w : h;
    float s = slider_size_ * ww + 0.5f;
    float t = (isHorizontal() ? h : w) * 0.5f + 1;
    if (type_ == Vertical_Knob || type_ == Horizontal_Knob)
        t += 4;
    if (s < t)
        s = t;
    float xx = value * (ww - s) + 0.5f;
    float xsl, ysl, wsl, hsl;
    if (isHorizontal()) {
        xsl = x + xx;
        wsl = s;
        ysl = y;
        hsl = h;
    } else {
        ysl = y + xx;
        hsl = s;
        xsl = x;
        wsl = w;
    }

    //
    drawBg();

    //
    Box_Style box1 = slider_box_;
    if (box1 == BS_NO_BOX)
        box1 = BS_UP_BOX;
    if (type_ == Vertical_Knob) {
        styled_box(*this, box1, xsl, ysl, wsl, hsl, Colors::gray());
        float d = (hsl - 4) * 0.5f;
        styled_box(*this, BS_THIN_DOWN_BOX, xsl + 2, ysl + d, wsl - 4, hsl - 2 * d, selection_col_);
    }
    else if (type_ == Horizontal_Knob) {
        styled_box(*this, box1, xsl, ysl, wsl, hsl, Colors::gray());
        float d = (wsl - 4) * 0.5f;
        styled_box(*this, BS_THIN_DOWN_BOX, xsl + d, ysl + 2, wsl - 2 * d, hsl - 4, selection_col_);
    }
    else {
        if (wsl > 0 && hsl > 0)
            styled_box(*this, box1, xsl, ysl, wsl, hsl, selection_col_);
    }
}

void Slider::drawBg()
{
    drawBox();

    int x = getAbsoluteX();
    int y = getAbsoluteY();
    int w = getWidth();
    int h = getHeight();

    Color black = /* active ? */ Colors::foreground() /* : Colors::inactive() */;
    if (type_ == Vertical_Knob)
        styled_box(*this, BS_THIN_DOWN_BOX, x + w / 2 - 2, y, 4, h, black);
    else if (type_ == Horizontal_Knob)
        styled_box(*this, BS_THIN_DOWN_BOX, x, y + h / 2 - 2, w, 4, black);
}

void Slider::drawBox()
{
    int x = getAbsoluteX();
    int y = getAbsoluteY();
    int w = getWidth();
    int h = getHeight();

    styled_box(*this, box_, x, y, w, h, col_);
}

bool Slider::onMouse(const MouseEvent &ev)
{
    // TODO
    

    return false;
}

bool Slider::onMotion(const MotionEvent &ev)
{
    // TODO
    

    return false;
}

bool Slider::onScroll(const ScrollEvent &ev)
{
    if (!contains(ev.pos))
        return false;

    float value = value_;
    float min = min_;
    float max = max_;

    const float d = (ev.mod & kModifierControl) ? 2000.0f : 200.0f;
    value += (max - min) / d * 10.f * ev.delta.getY();

    value = (value < min) ? min : value;
    value = (value > max) ? max : value;

    setValue(value, true);
    return false;

    return false;
}
