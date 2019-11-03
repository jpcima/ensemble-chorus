#include "LookAndFeel.hpp"
#include "Cairo.hpp"
#include "ui/Cairo++.h"
#include <complex>
#include <cmath>

typedef std::complex<double> cdouble;

ColorRGBA8 LookAndFeel::getTextColor() const
{
    return ColorRGBA8{0x00, 0x00, 0x00, 0xff};
}

ColorRGBA8 LookAndFeel::getBackgroundColor() const
{
    return ColorRGBA8{0xc7, 0xc7, 0xc7, 0xff};
}

ColorRGBA8 LookAndFeel::getActiveFillColor() const
{
    return ColorRGBA8{0x68, 0x9a, 0xcf, 0xff};
}

ColorRGBA8 LookAndFeel::getPassiveFillColor() const
{
    return ColorRGBA8{0x40, 0x40, 0x40, 0xff};
}

void LookAndFeel::drawHorizontalSlider(const GraphicsContext &gc, RectF bounds, double fill)
{
    cairo_t *cr = gc.cairo;
    RectF r = bounds.off_by({0.5, 0.5});
    ColorRGBA8 fillColor = getActiveFillColor();
    ColorRGBA8 passiveFillColor = getPassiveFillColor();

    cairo_set_source_rgba8(cr, fillColor);
    cairo_rectangle(cr, r.resized({fill * r.w, r.h}).reduced({0.5, 0.5}));
    cairo_fill(cr);

    cairo_set_source_rgba8(cr, passiveFillColor);
    cairo_rectangle(cr, r);
    cairo_stroke(cr);
}

void LookAndFeel::drawVerticalSlider(const GraphicsContext &gc, RectF bounds, double fill)
{
    cairo_t *cr = gc.cairo;
    RectF r = bounds.off_by({0.5, 0.5});
    ColorRGBA8 fillColor = getActiveFillColor();
    ColorRGBA8 passiveFillColor = getPassiveFillColor();

    cairo_set_source_rgba8(cr, fillColor);
    cairo_rectangle(cr, r.resized({r.w, fill * r.h}).off_by({0.0, r.h - fill * r.h}).reduced({0.5, 0.5}));
    cairo_fill(cr);

    cairo_set_source_rgba8(cr, passiveFillColor);
    cairo_rectangle(cr, r);
    cairo_stroke(cr);
}

void LookAndFeel::drawRadialSlider(const GraphicsContext &gc, RectF bounds, double angleMin, double angleMax, double fill)
{
    cairo_t *cr = gc.cairo;
    ColorRGBA8 fillColor = getActiveFillColor();
    ColorRGBA8 passiveFillColor = getPassiveFillColor();

    cairo_save(cr);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

    double a1 = angleMin - 0.5 * M_PI;
    double a2 = angleMax - 0.5 * M_PI;

    int size = std::min(bounds.w, bounds.h);
    double arcRadius = size * 0.35;

    cairo_set_source_rgba8(cr, passiveFillColor);
    cairo_set_line_width(cr, 8.0);
    cairo_new_path(cr);
    cairo_arc(cr, 0.5 * bounds.w, 0.5 * bounds.h, arcRadius, a1, a2);
    cairo_stroke(cr);

    a2 = angleMin + fill * (angleMax - angleMin) - 0.5 * M_PI;

    cairo_set_source_rgba8(cr, fillColor);
    cairo_set_line_width(cr, 3.0);
    cairo_new_path(cr);
    cairo_arc(cr, 0.5 * bounds.w, 0.5 * bounds.h, arcRadius, a1, a2);
    cairo_stroke(cr);

    cdouble point = std::polar(arcRadius, a2) + cdouble{0.5 * bounds.w, 0.5 * bounds.h};
    cairo_new_path(cr);
    cairo_arc(cr, point.real(), point.imag(), 5.0, 0, 2 * M_PI);
    cairo_fill_preserve(cr);
    cairo_set_line_width(cr, 1.0);
    cairo_set_source_rgba8(cr, passiveFillColor);
    cairo_stroke(cr);

    cairo_restore(cr);
}

void LookAndFeel::drawToggleButton(const GraphicsContext &gc, RectF bounds, bool checked)
{
    cairo_t *cr = gc.cairo;
    ColorRGBA8 fillColor = getActiveFillColor();
    ColorRGBA8 passiveFillColor = getPassiveFillColor();

    RectF r = bounds.off_by({0.5, 0.5});
    cairo_set_source_rgba8(cr, passiveFillColor);
    cairo_rectangle(cr, r);
    cairo_stroke(cr);

    if (checked) {
        cairo_set_source_rgba8(cr, fillColor);
        cairo_rectangle(cr, r.reduced({0.5, 0.5}));
        cairo_fill(cr);
    }
}
