//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "../graphics/box.h"
#include "../graphics/colors.h"
#include <dgl/NanoVG.hpp>

class Slider : public NanoWidget {
public:
    struct Callback {
        virtual ~Callback() {}
        virtual void sliderDragStarted(Slider *slider) = 0;
        virtual void sliderDragFinished(Slider *slider) = 0;
        virtual void sliderValueChanged(Slider *slider, float value) = 0;
    };

    enum Type {
        Vertical,
        Horizontal,
        Vertical_Knob,
        Horizontal_Knob,
    };

    explicit Slider(NanoWidget *groupWidget);

    float getValue() const
        { return value_; }

    void setRange(float min, float max);
    void setValue(float value, bool sendCallback = false);

    void setCallback(Callback *cb)
        { callback_ = cb; }

    Type getType() const
        { return type_; }
    void setType(Type t);

    bool isHorizontal() const
        { return type_ == Horizontal || type_ == Horizontal_Knob; }
    bool isVertical() const
        { return !isHorizontal(); }

protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &ev) override;
    bool onMotion(const MotionEvent &ev) override;
    bool onScroll(const ScrollEvent &ev) override;

private:
    void drawInBounds(float x, float y, float w, float h);
    void drawBg();
    void drawBox();
    float value_ = 0;
    float min_ = 0;
    float max_ = 1;
    Callback *callback_ = nullptr;
    Color col_ = Colors::background();
    Color selection_col_ = Colors::gray();
    Box_Style box_ = BS_DOWN_BOX;
    Box_Style slider_box_ = BS_UP_BOX;
    Type type_ = Horizontal;
    double slider_size_ = 0;
};
