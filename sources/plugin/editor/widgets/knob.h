//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <dgl/NanoVG.hpp>

class Knob : public NanoWidget {
public:
    struct Callback {
        virtual ~Callback() {}
        virtual void knobDragStarted(Knob *knob) = 0;
        virtual void knobDragFinished(Knob *knob) = 0;
        virtual void knobValueChanged(Knob *knob, float value) = 0;
    };

    explicit Knob(NanoWidget *groupWidget);

    float getValue() const
        { return value_; }

    void setRange(float min, float max);
    void setValue(float value, bool sendCallback = false);

    void setCallback(Callback *cb)
        { callback_ = cb; }

    void setColor(Color c)
        { col_ = c; }
    void setCursorColor(Color c)
        { cur_ = c; }
    void setBackgroundColor(Color c)
        { bg_ = c; }

protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &ev) override;
    bool onMotion(const MotionEvent &ev) override;
    bool onScroll(const ScrollEvent &ev) override;

private:
    static Color shadow(int offs, Color c);
    void drawArc(float x, float y, float r, float a1, float a2, Color c);
    void drawPie(float x, float y, float r, float a1, float a2, Color c);
    void drawCircle(float x, float y, float r, Color c);
    void drawCircleOutline(float x, float y, float r, Color c);
    void drawScale(float x, float y, float side);
    void drawCursor(float x, float y, float side);
    float value_ = 0;
    float min_ = 0;
    float max_ = 1;
    Callback *callback_ = nullptr;
    bool dragging_ = false;
    unsigned scaleticks_ = 10;
    float percent_ = 0.3;
    Color col_{0xc0, 0xc0, 0xc0};
    Color cur_{0x55, 0x55, 0x55};
    Color bg_{0xc0, 0xc0, 0xc0};
    static constexpr int a1 = 35;
    static constexpr int a2 = 325;
};
