#pragma once
#include "Window.hpp"
#include "ui/Color.h"
#include "ui/Geometry.h"

class LookAndFeel {
public:
    virtual ~LookAndFeel() {}

    virtual ColorRGBA8 getTextColor() const;
    virtual ColorRGBA8 getBackgroundColor() const;
    virtual ColorRGBA8 getActiveFillColor() const;
    virtual ColorRGBA8 getPassiveFillColor() const;

    virtual void drawHorizontalSlider(const GraphicsContext &gc, RectF bounds, double fill);
    virtual void drawVerticalSlider(const GraphicsContext &gc, RectF bounds, double fill);
    virtual void drawRadialSlider(const GraphicsContext &gc, RectF bounds, double angleMin, double angleMax, double fill);

    virtual void drawToggleButton(const GraphicsContext &gc, RectF bounds, bool checked);
};
