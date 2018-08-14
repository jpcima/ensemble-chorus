//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "../graphics/text.h"
#include <dgl/NanoVG.hpp>
#include <string>

class Button : public NanoWidget {
public:
    struct Callback {
        virtual ~Callback() {}
        virtual void buttonValueChanged(Button *btn, bool value) = 0;
    };

    explicit Button(NanoWidget *groupWidget);

    bool getValue() const
        { return value_; }

    void setValue(bool value, bool sendCallback = false);

    void setColor(Color c);
    void setLabel(const char *label);
    void setFont(FontId font);
    void setFontSize(float size);
    void setTextStyle(Text_Style ts);

    void setCallback(Callback *cb)
        { callback_ = cb; }

protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &ev) override;
    bool onMotion(const MotionEvent &ev) override;

private:
    bool value_ = false;
    std::string label_;
    FontId font_ = 0;
    float fontsize_ = 15.0f;
    Text_Style ts_ = TS_NORMAL;
    Color col_{0xc0, 0xc0, 0xc0};
    Callback *callback_ = nullptr;
    int pressed_ = -1;
    bool hovered_ = false;
};
