//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "Fl_Valuator_Ex.h"
#include <FL/Fl.H>

template <class W>
Fl_Valuator_Ex<W>::Fl_Valuator_Ex(int x, int y, int w, int h, const char *l)
    : W(x, y, w, h, l)
{
}

template <class W>
int Fl_Valuator_Ex<W>::handle(int event)
{
    if (event == FL_MOUSEWHEEL) {
        this->value(this->clamp(this->increment(this->value(), Fl::event_dy())));
        return 1;
    }
    return W::handle(event);
}
