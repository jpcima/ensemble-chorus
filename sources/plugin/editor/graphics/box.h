//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <dgl/NanoVG.hpp>

enum Box_Style {
    BS_SHADOW_BOX,
    BS_DOWN_BOX,
    BS_UP_BOX,
    BS_THIN_DOWN_BOX,
    BS_THIN_UP_BOX,
    //
    BS_SHADOW_FRAME,
    BS_DOWN_FRAME,
    BS_UP_FRAME,
    BS_THIN_DOWN_FRAME,
    BS_THIN_UP_FRAME,
    //
    BS_NO_BOX = -1,
};

void styled_box(NanoVG &g, Box_Style bs, float x, float y, float w, float h, Color c);
void styled_box_deltas(Box_Style bs, float *dx, float *dy, float *dw, float *dh);
