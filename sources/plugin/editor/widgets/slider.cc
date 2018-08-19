//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "slider.h"
#include <algorithm>
#include <cassert>

Slider::Slider(NanoWidget *groupWidget)
    : NanoWidget(groupWidget)
{
    setDrawingBuffered(true);
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
    value = std::max(value, min_);
    value = std::min(value, max_);
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
    bool inverted = !isHorizontal();

    if (inverted)
        value = 1 - value;

    //
    Type type = type_;
    bool horizontal = isHorizontal();
    float ww = horizontal ? w : h;
    float s = slider_size_ * ww + 0.5f;
    float t = (horizontal ? h : w) * 0.5f + 1;
    if (type == Vertical_Knob || type == Horizontal_Knob)
        t += 4;
    if (s < t)
        s = t;
    float xx = value * (ww - s) + 0.5f;
    float xsl, ysl, wsl, hsl;
    if (horizontal) {
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
    Color col = selection_col_;
    if (type == Vertical_Knob) {
        styled_box(*this, box1, xsl, ysl, wsl, hsl, Colors::gray());
        float d = (hsl - 4) * 0.5f;
        styled_box(*this, BS_THIN_DOWN_BOX, xsl + 2, ysl + d, wsl - 4, hsl - 2 * d, col);
    }
    else if (type == Horizontal_Knob) {
        styled_box(*this, box1, xsl, ysl, wsl, hsl, Colors::gray());
        float d = (wsl - 4) * 0.5f;
        styled_box(*this, BS_THIN_DOWN_BOX, xsl + d, ysl + 2, wsl - 2 * d, hsl - 4, col);
    }
    else {
        if (wsl > 0 && hsl > 0)
            styled_box(*this, box1, xsl, ysl, wsl, hsl, col);
    }
}

void Slider::drawBg()
{
    drawBox();

    int x = getAbsoluteX();
    int y = getAbsoluteY();
    int w = getWidth();
    int h = getHeight();

    Type type = type_;
    Color black = /* active ? */ Colors::foreground() /* : Colors::inactive() */;
    if (type == Vertical_Knob)
        styled_box(*this, BS_THIN_DOWN_BOX, x + w / 2 - 2, y, 4, h, black);
    else if (type == Horizontal_Knob)
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
    if (ev.button != 1)
        return false;

    if (ev.press) {
        if (!contains(ev.pos))
            return false;

        return pushOrDrag(ev.pos.getX(), ev.pos.getY(), !dragging_);
    }
    else if (dragging_) {
        if (callback_)
            callback_->sliderDragFinished(this);

        dragging_ = false;
        return true;
    }

    return false;
}

bool Slider::onMotion(const MotionEvent &ev)
{
    if (!dragging_)
        return false;

    return pushOrDrag(ev.pos.getX(), ev.pos.getY(), false);
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

bool Slider::pushOrDrag(float evx, float evy, bool isPush)
{
    if (!isPush && !dragging_)
        return false;

    float value = value_;
    float dispvalue = value;
    bool horizontal = isHorizontal();
    bool inverted = !horizontal;
    Type type = type_;

    if (inverted)
        dispvalue = 1 - dispvalue;

    float w = getWidth();
    float h = getHeight();
    float ww = horizontal ? w : h;
    float mx = horizontal ? evx : evy;

    float s = slider_size_ * ww + 0.5f;
    if (s >= ww)
        return 0;
    float t = (horizontal ? h : w) * 0.5f + 1;
    if (type == Vertical_Knob || type == Horizontal_Knob)
        t += 4;
    if (s < t)
        s = t;

    float &offcenter = offcenter_;
    if (isPush) {
        float xx = dispvalue * (ww - s) + 0.5f;
        offcenter = mx - xx;
        if (offcenter < 0)
            offcenter = 0;
        else if (offcenter > s)
            offcenter = s;
        else {
            if (!dragging_) {
                dragging_ = true;
                if (callback_)
                    callback_->sliderDragStarted(this);
            }
            return true;
        }
    }

    float xx = mx - offcenter;
    float v = 0;
    float min = min_;
    float max = max_;
    bool tryAgain = true;
    while (tryAgain) {
        tryAgain = false;
        if (xx < 0) {
            xx = 0;
            offcenter = std::max(mx, 0.0f);
        } else if (xx > (ww - s)) {
            xx = ww - s;
            offcenter = std::min(mx - xx, s);
        }
        float r = xx / (ww - s);
        if (inverted)
            r = 1 - r;
        v = /* std::round */(r * (max - min) + min);
        // make sure a click outside the sliderbar moves it
        if (isPush && v == value) {
            offcenter = s * 0.5f;
            isPush = false;
            tryAgain = true;
        }
    }

    setValue(v, true);

    if (!dragging_) {
        dragging_ = true;
        if (callback_)
            callback_->sliderDragStarted(this);
    }

    return true;
}
