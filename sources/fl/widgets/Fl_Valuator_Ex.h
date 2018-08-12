//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <FL/Fl_Valuator.H>

template <class W>
class Fl_Valuator_Ex : public W {
public:
    Fl_Valuator_Ex(int x, int y, int w, int h, const char *l = nullptr);
    int handle(int event) override;
};

#include "Fl_Valuator_Ex.tcc"
