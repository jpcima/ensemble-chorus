#include "ModulatorView.h"
#include "ui/components/LookAndFeel.hpp"
#include "ui/Cairo++.h"
#include "Window.hpp"
#include "Cairo.hpp"

ModulatorView::ModulatorView(Widget *group, LookAndFeel &lnf)
    : BasicWidget(group),
      fLookAndFeel(lnf)
{
}

void ModulatorView::setValue1(double value)
{
    if (fValue1 == value)
        return;

    fValue1 = value;
    repaint();
}

void ModulatorView::setValue2(double value)
{
    if (fValue2 == value)
        return;

    fValue2 = value;
    repaint();
}

void ModulatorView::onDisplay()
{
    cairo_t *cr = getParentWindow().getGraphicsContext().cairo;
    LookAndFeel &lnf = fLookAndFeel;

    int w = getWidth();
    int h = getHeight();
    RectF r = RectF{0, 0, (double)w, (double)h}.off_by({0.5, 0.5});

    ColorRGBA8 fillColor = lnf.getActiveFillColor();
    ColorRGBA8 passiveFillColor = lnf.getPassiveFillColor();

    cairo_set_source_rgba8(cr, passiveFillColor);
    cairo_rectangle(cr, r);
    cairo_stroke(cr);

    double radius = 3.0;
    double widthRatio = 0.6;

    cairo_new_path(cr);
    cairo_arc(cr, w * (fValue1 * widthRatio + 0.5 * (1.0 - widthRatio)), 0.25 * h, radius, 0.0, 2.0 * M_PI);
    cairo_set_source_rgba8(cr, fillColor);
    cairo_fill_preserve(cr);
    cairo_set_source_rgba8(cr, passiveFillColor);
    cairo_stroke(cr);

    cairo_new_path(cr);
    cairo_arc(cr, w * (fValue2 * widthRatio + 0.5 * (1.0 - widthRatio)), 0.75 * h, radius, 0.0, 2.0 * M_PI);
    cairo_set_source_rgba8(cr, fillColor);
    cairo_fill_preserve(cr);
    cairo_set_source_rgba8(cr, passiveFillColor);
    cairo_stroke(cr);
}
