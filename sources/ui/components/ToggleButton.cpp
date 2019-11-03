#include "ToggleButton.hpp"
#include "LookAndFeel.hpp"
#include "Window.hpp"
#include "Cairo.hpp"
#include "ui/Geometry.h"
#include "ui/Cairo++.h"

ToggleButton::ToggleButton(Widget *group, LookAndFeel &lnf)
    : BasicWidget(group),
      fLookAndFeel(lnf)
{
}

void ToggleButton::setValue(bool value)
{
    if (fValue == value)
        return;

    fValue = value;
    if (ValueChangedCallback && fValueNotify)
        ValueChangedCallback(value);
    repaint();
}

void ToggleButton::setValueNotified(bool notified)
{
    fValueNotify = notified;
}

void ToggleButton::setHasInvertedAppearance(bool inv)
{
    if (fHasInvertedAppearance == inv)
        return;

    fHasInvertedAppearance = inv;
    repaint();
}

bool ToggleButton::onMouse(const MouseEvent &event)
{
    DGL::Size<uint> wsize = getSize();
    DGL::Point<int> mpos = event.pos;

    bool inside = mpos.getX() >= 0 && mpos.getY() >= 0 &&
        (unsigned)mpos.getX() < wsize.getWidth() && (unsigned)mpos.getY() < wsize.getHeight();

    if (event.press && event.button == 1 && inside) {
        if (fInteractible) {
            fIsPressed = true;
            repaint();
        }
        return true;
    }
    else if (!event.press && event.button == 1) {
        if (fIsPressed) {
            fIsPressed = false;
            if (inside)
                setValue(!fValue);
            repaint();
            return true;
        }
    }

    return false;
}

void ToggleButton::onDisplay()
{
    const GraphicsContext &gc = getParentWindow().getGraphicsContext();
    LookAndFeel &lnf = fLookAndFeel;

    bool checked = fValue ^ fIsPressed;
    if (fHasInvertedAppearance)
        checked = !checked;

    RectF bounds{0, 0, (double)getWidth(), (double)getHeight()};
    lnf.drawToggleButton(gc, bounds, checked);
}
