//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "modulator_visu.h"
#include <FL/fl_draw.H>

Modulator_Visu::Modulator_Visu(int x, int y, int w, int h, const char *l)
    : Fl_Box(x, y, w, h, l)
{
}

void Modulator_Visu::value(double v1, double v2)
{
    if (value1_ == v1 && value2_ == v2)
        return;

    value1_ = v1;
    value2_ = v2;
    redraw();
}

void Modulator_Visu::draw()
{
    Fl_Box::draw();

    int x = this->x(), y = this->y();
    int w = this->w(), h = this->h();

    fl_color(FL_BLACK);

    double r1 = 2.0;
    double r2 = 1.5;
    double xpad = 8.0;
    double x1 = x + xpad + (w - 2 * xpad) * value1_;
    double x2 = x + xpad + (w - 2 * xpad) * value2_;
    double y1 = y + h * (1.0 / 3.0);
    double y2 = y + h * (2.0 / 3.0);

    fl_circle(x1, y1, r1);
    fl_circle(x2, y2, r2);
}
