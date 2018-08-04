//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <FL/Fl_Box.H>

class Modulator_Visu : public Fl_Box {
public:
    Modulator_Visu(int x, int y, int w, int h, const char *l = nullptr);

    void value(double v1, double v2);

    void draw() override;

private:
    double value1_ = 0.5;
    double value2_ = 0.5;
};
