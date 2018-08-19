//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "button.h"
#include "../graphics/box.h"
#include "../graphics/colors.h"

Button::Button(NanoWidget *groupWidget)
    : NanoWidget(groupWidget)
{
    setDrawingBuffered(true);
    setSize(50, 50);
}

void Button::setValue(bool value, bool sendCallback)
{
    if (value == value_)
        return;

    value_ = value;
    repaint();

    if (sendCallback && callback_)
        callback_->buttonValueChanged(this, value);
}

void Button::setColor(Color c)
{
    col_ = c;
    repaint();
}

void Button::setLabel(const char *label)
{
    if (!label)
        label_.clear();
    else
        label_.assign(label);
    repaint();
}

void Button::setFont(FontId font)
{
    font_ = font;
    repaint();
}

void Button::setFontSize(float size)
{
    fontsize_ = size;
    repaint();
}

void Button::setTextStyle(Text_Style ts)
{
    ts_ = ts;
    repaint();
}

void Button::onNanoDisplay()
{
    // TODO
    float x = getAbsoluteX();
    float y = getAbsoluteY();
    float w = getWidth();
    float h = getHeight();
    Color col = col_;

    bool value = value_;
    if (pressed_ != -1 && hovered_)
        value = !value;

    Box_Style bs = value ? BS_DOWN_BOX : BS_UP_BOX;
    styled_box(*this, bs, x, y, w, h, col);

    const std::string &label = label_;
    if (!label.empty()) {
        fontFaceId(font_);
        fontSize(fontsize_);
        bounded_text(*this, ts_, x, y, w, h, ALIGN_CENTER|ALIGN_MIDDLE, label.c_str(), Colors::gray0());
    }
}

bool Button::onMouse(const MouseEvent &ev)
{
    if (ev.press && pressed_ == -1) {
        if (contains(ev.pos)) {
            pressed_ = ev.button;
            repaint();
            return true;
        }
    }
    else if (!ev.press && pressed_ == ev.button) {
        pressed_ = -1;
        if (contains(ev.pos))
            setValue(!value_, true);
        else
            repaint();
        return true;
    }

    return false;
}

bool Button::onMotion(const MotionEvent &ev)
{
    bool hovered = contains(ev.pos);
    if (hovered != hovered_) {
        hovered_ = hovered;
        repaint();
    }

    return false;
}
