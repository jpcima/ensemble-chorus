#pragma once
#include "Widget.hpp"
#include "ui/Geometry.h"

class BasicWidget : public Widget {
public:
    explicit BasicWidget(Widget *group) : Widget(group) {}
    virtual ~BasicWidget() {}

    Rect getBounds() const;
    void setBounds(const Rect &rect);
};
