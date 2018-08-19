//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "knob.h"
#include <algorithm>
#include <cmath>
#include <cassert>

Knob::Knob(NanoWidget *groupWidget)
    : NanoWidget(groupWidget)
{
    setDrawingBuffered(true);
    setSize(50, 50);
}

void Knob::setRange(float min, float max)
{
    assert(min < max);

    float value = value_;
    min_ = min;
    max_ = max;

    if (value < min) {
        value = value_ = min;
        repaint();

        if (callback_)
            callback_->knobValueChanged(this, value);
    }
    else if (value > max) {
        value = value_ = max;
        repaint();

        if (callback_)
            callback_->knobValueChanged(this, value);
    }
}

void Knob::setValue(float value, bool sendCallback)
{
    value = std::max(value, min_);
    value = std::min(value, max_);
    if (value == value_)
        return;

    value_ = value;
    repaint();

    if (sendCallback && callback_)
        callback_->knobValueChanged(this, value);
}

void Knob::onNanoDisplay()
{
    float ox = getAbsoluteX();
    float oy = getAbsoluteY();
    float w = getWidth();
    float h = getHeight();

    float side;
    if (w > h) {
        side = h;
        ox += 0.5f * (w - h);
    }
    else {
        side = w;
        oy += 0.5f * (h - w);
    }
    float r = 0.5f * side;

    const Color col = col_;
    const Color bg = bg_;
    Color sh;

    sh = shadow(-60, bg);
    drawCircle(ox + 9, oy + 9, r - 6, sh);
    drawScale(ox, oy, side);

    sh = shadow(7, col);
    drawPie(ox + 6, oy + 6, r - 6, 40, 50, sh);
    drawPie(ox + 6, oy + 6, r - 6, 260, 270, sh);

    sh = shadow(15, col);
    drawPie(ox + 6, oy + 6, r - 6, 50, 70, sh);
    drawPie(ox + 6, oy + 6, r - 6, 230, 260, sh);

    sh = shadow(25, col);
    drawPie(ox + 6, oy + 6, r - 6, 70, 80, sh);
    drawPie(ox + 6, oy + 6, r - 6, 220, 230, sh);

    sh = shadow(30, col);
    drawPie(ox + 6, oy + 6, r - 6, 80, 220, sh);

    sh = shadow(-7, col);
    drawPie(ox + 6, oy + 6, r - 6, 30, 40, sh);
    drawPie(ox + 6, oy + 6, r - 6, 270, 280, sh);

    sh = shadow(-15, col);
    drawPie(ox + 6, oy + 6, r - 6, 280, 400, sh);
    sh = shadow(-25, col);
    drawPie(ox + 6, oy + 6, r - 6, 290, 390, sh);

    sh = Color(0, 0, 0);
    drawCircleOutline(ox + 6, oy + 6, r - 5.5f, sh);
    drawCircle(ox + 10, oy + 10, r - 10, col);

    ///
    sh = shadow(10, col);
    drawPie(ox + 10, oy + 10, r - 10, 110, 150, sh);
    drawPie(ox + 10, oy + 10, r - 10, 290, 330, sh);
    sh = shadow(17, col);
    drawPie(ox + 10, oy + 10, r - 10, 120, 140, sh);
    drawPie(ox + 10, oy + 10, r - 10, 300, 320, sh);
    sh = shadow(25, col);
    drawPie(ox + 10, oy + 10, r - 10, 127, 133, sh);
    drawPie(ox + 10, oy + 10, r - 10, 307, 313, sh);
    drawCursor(ox, oy, side);
}

bool Knob::onMouse(const MouseEvent &ev)
{
    if (ev.button != 1)
        return false;

    if (ev.press)
    {
        if (!contains(ev.pos))
            return false;

        dragging_ = true;
        if (callback_ != nullptr)
            callback_->knobDragStarted(this);

        return true;
    }
    else if (dragging_) {
        if (callback_)
            callback_->knobDragFinished(this);

        dragging_ = false;
        return true;
    }

    return false;
}

bool Knob::onMotion(const MotionEvent &ev)
{
    if (!dragging_)
        return false;

    float value = value_;
    float min = min_;
    float max = max_;

    float mx = ev.pos.getX() - 0.5f * getWidth();
    float my = ev.pos.getY() - 0.5f * getHeight();
    float angle = 270 - radToDeg(std::atan2(-my, mx));
    float oldangle = (a2 - a1) * (value - min) / (max - min) + a1;
    while (angle < oldangle - 180) angle += 360;
    while (angle > oldangle + 180) angle -= 360;

    if ((a1 < a2) ? (angle <= a1) : (angle >= a1))
        value = min;
    else if ((a1 < a2) ? (angle >= a2) : (angle <= a2))
        value = max;
    else
        value = min + (max - min) * (angle - a1) / (a2 - a1);
    value = (value < min) ? min : value;
    value = (value > max) ? max : value;

    setValue(value, true);
    return true;
}

bool Knob::onScroll(const ScrollEvent &ev)
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
}

Color Knob::shadow(int offs, Color c)
{
    float off = offs * (1.0f / 255);
    return Color(c.red + off, c.green + off, c.blue + off, c.alpha);
}

void Knob::drawArc(float x, float y, float r, float a1, float a2, Color c)
{
    float cx = x + r;
    float cy = y + r;
    beginPath();
    a1 = degToRad(360 - a1);
    a2 = degToRad(360 - a2);
    arc(cx, cy, r, a1, a2, CCW);
    strokeColor(c);
    stroke();
}

void Knob::drawPie(float x, float y, float r, float a1, float a2, Color c)
{
    float cx = x + r;
    float cy = y + r;
    beginPath();
    moveTo(cx, cy);
    a1 = degToRad(360 - a1);
    a2 = degToRad(360 - a2);
    arc(cx, cy, r, a1, a2, CCW);
    fillColor(c);
    fill();
}

void Knob::drawCircle(float x, float y, float r, Color c)
{
    float cx = x + r;
    float cy = y + r;
    beginPath();
    circle(cx, cy, r);
    fillColor(c);
    fill();
}

void Knob::drawCircleOutline(float x, float y, float r, Color c)
{
    float cx = x + r;
    float cy = y + r;
    beginPath();
    circle(cx, cy, r);
    strokeColor(c);
    stroke();
}

void Knob::drawScale(float ox, float oy, float side)
{
    unsigned scaleticks = scaleticks_;
    float rds = 0.5f * side;
    float cx = ox + rds;
    float cy = oy + rds;

    if (scaleticks == 0) return;
    float a_step = (float)M_PI * (10.0f / 6.0f) / scaleticks;
    float a_orig = -((float)M_PI/3.0f);
    for (unsigned a = 0; a <= scaleticks; a++)
    {
        float na = a_orig + a * a_step;
        float ca = std::cos(na);
        float sa = std::sin(na);
        float x1 = cx + rds * ca;
        float y1 = cy - rds * sa;
        float x2 = cx + (rds - 6) * ca;
        float y2 = cy - (rds - 6) * sa;

        beginPath();
        moveTo(x1, y1);
        lineTo(x2, y2);
        strokeColor(0.0f, 0.0f, 0.0f);
        stroke();

        beginPath();
        if (sa * ca >=0) {
            moveTo(x1 + 1, y1 + 1);
            lineTo(x2 + 1, y2 + 1);
        }
        else {
            moveTo(x1 + 1, y1 - 1);
            lineTo(x2 + 1, y2 - 1);
        }
        strokeColor(1.0f, 1.0f, 1.0f);
        stroke();
    }
}

void Knob::drawCursor(float ox, float oy, float side)
{
    float percent = percent_;
    float rds = 0.5f * (side - 20);
    float cur = percent * 0.5f * rds;
    float cx = ox + 0.5f * side;
    float cy = oy + 0.5f * side;
    float value = value_;
    float min = min_;
    float max = max_;
    value = (value - min) / (max - min);
    float angle = (a2 - a1) * value + a1;

    save();
    scale(1, 1);
    translate(cx, cy);
    rotate(degToRad(angle));
    translate(0, rds - cur - 2.0f);

    beginPath();
    circle(0, 0, cur);
    fillColor(cur_);
    fill();
    beginPath();
    circle(0, 0, cur);
    strokeColor(Color(0.0f, 0.0f, 0.0f));
    stroke();

    restore();
}
