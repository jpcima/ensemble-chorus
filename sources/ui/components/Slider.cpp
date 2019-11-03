#include "Slider.hpp"
#include "Window.hpp"
#include "Cairo.hpp"
#include "ui/components/LookAndFeel.hpp"
#include "ui/Geometry.h"
#include "ui/Cairo++.h"
#include <cmath>

///
Slider::Slider(Widget *group, LookAndFeel &lnf)
    : BasicWidget(group),
      fLookAndFeel(lnf)
{
}

void Slider::setValue(double value)
{
    value = clampToBounds(value);

    if (fValue == value)
        return;

    fValue = value;
    if (ValueChangedCallback && fValueNotify)
        ValueChangedCallback(value);
    repaint();
}

void Slider::setValueNotified(bool notified)
{
    fValueNotify = notified;
}

void Slider::setValueBounds(double v1, double v2)
{
    fValueBound1 = v1;
    fValueBound2 = v2;
    setValue(fValue);
}

void Slider::setDefaultValue(double vd)
{
    fDefaultValue = vd;
    fHasDefaultValue = true;
}

void Slider::unsetDefaultValue()
{
    fHasDefaultValue = false;
}

void Slider::setNumSteps(unsigned numSteps)
{
    fNumSteps = numSteps;
}

void Slider::setOrientation(Orientation ori)
{
    if (fOrientation == ori)
        return;

    fOrientation = ori;
    repaint();
}

void Slider::setLogarithmic(bool loga)
{
    if (fLogarithm == loga)
        return;

    fLogarithm = loga;
    repaint();
}

bool Slider::onMouse(const MouseEvent &event)
{
    DGL::Size<uint> wsize = getSize();
    DGL::Point<int> mpos = event.pos;

    bool insideX = mpos.getX() >= 0 && (unsigned)mpos.getX() < wsize.getWidth();
    bool insideY = mpos.getY() >= 0 && (unsigned)mpos.getY() < wsize.getHeight();

    if (!fIsDragging && event.press && event.button == 1) {
        if (!insideX || !insideY)
            return false;

        fIsDragging = true;

        double fill = 0;
        if (fOrientation == Horizontal) {
            fill = mpos.getX() / (double)wsize.getWidth();
            setValue(proportionToValue(fill));
        }
        else if (fOrientation == Vertical) {
            fill = 1.0 - (mpos.getY() / (double)wsize.getHeight());
            setValue(proportionToValue(fill));
        }

        return true;
    }
    else if (fIsDragging && !event.press && event.button == 1) {
        fIsDragging = false;
        return true;
    }
    else if (!fIsDragging && event.press && event.button == 3) {
        if (!insideX || !insideY)
            return false;

        if (fHasDefaultValue)
            setValue(fDefaultValue);
        return true;
    }

    return false;
}

bool Slider::onMotion(const MotionEvent &event)
{
    DGL::Size<uint> wsize = getSize();
    DGL::Point<int> mpos = event.pos;

    if (fIsDragging) {
        double fill = 0;
        bool doNotSet = false;
        if (fOrientation == Horizontal)
            fill = mpos.getX() / (double)wsize.getWidth();
        else if (fOrientation == Vertical)
            fill = 1.0 - (mpos.getY() / (double)wsize.getHeight());
        else if (fOrientation == Radial) {
            double dx = mpos.getX() - 0.5 * getWidth();
            double dy = mpos.getY() - 0.5 * getHeight();
            if (dx * dx + dy * dy < 100.0)
                doNotSet = true;
            else {
                double angle = std::atan2(dx, -dy);
                angle = std::max(angle, fAngleMin);
                angle = std::min(angle, fAngleMax);
                fill = (angle - fAngleMin) / (fAngleMax - fAngleMin);
            }
        }
        fill = (fill < 0) ? 0 : fill;
        fill = (fill > 1) ? 1 : fill;
        if (!doNotSet)
            setValue(proportionToValue(fill));
        return true;
    }

    return false;
}

bool Slider::onScroll(const ScrollEvent &event)
{
    DGL::Size<uint> wsize = getSize();
    DGL::Point<int> mpos = event.pos;

    bool inside =
        mpos.getX() >= 0 && mpos.getY() >= 0 &&
        (unsigned)mpos.getX() < wsize.getWidth() && (unsigned)mpos.getY() < wsize.getHeight();

    if (inside) {
        double amount = event.delta.getY() - event.delta.getX();
        double prop = std::min(1.0, std::max(0.0, valueToProportion(fValue) + amount / fNumSteps));
        setValue(proportionToValue(prop));
        return true;
    }

    return false;
}

void Slider::onDisplay()
{
    const GraphicsContext &gc = getParentWindow().getGraphicsContext();
    LookAndFeel &lnf = fLookAndFeel;

    RectF bounds{0, 0, (double)getWidth(), (double)getHeight()};

    //
    double value = fValue;
    double fill = valueToProportion(value);

    //
    switch (fOrientation) {
    default:
    case Horizontal:
        lnf.drawHorizontalSlider(gc, bounds, fill);
        break;
    case Vertical:
        lnf.drawVerticalSlider(gc, bounds, fill);
        break;
    case Radial:
        lnf.drawRadialSlider(gc, bounds, fAngleMin, fAngleMax, fill);
        break;
    }
}

double Slider::clampToBounds(double value)
{
    double vmin = fValueBound1;
    double vmax = fValueBound2;
    if (vmin > vmax)
        std::swap(vmin, vmax);

    value = (value < vmin) ? vmin : value;
    value = (value > vmax) ? vmax : value;
    return value;
}

double Slider::valueToProportion(double value)
{
    double v1 = fValueBound1;
    double v2 = fValueBound2;
    if (fLogarithm)
        return std::log(value / v1) / std::log(v2 / v1);
    else
        return (v1 != v2) ? ((value - v1) / (v2 - v1)) : 0.0;
}

double Slider::proportionToValue(double prop)
{
    double v1 = fValueBound1;
    double v2 = fValueBound2;
    if (fLogarithm)
        return v1 * std::pow(v2 / v1, prop);
    else
        return v1 + prop * (v2 - v1);
}
