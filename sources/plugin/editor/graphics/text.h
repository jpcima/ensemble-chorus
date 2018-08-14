//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <dgl/NanoVG.hpp>

enum Text_Style {
    TS_NORMAL,
    TS_SHADOW,
    TS_ENGRAVED,
    TS_EMBOSSED,
};

void bounded_text(NanoVG &g, Text_Style ts, float x, float y, float w, float h, int align, const char *text, Color c);
void styled_text(NanoVG &g, Text_Style ts, float x, float y, float w, const char *text, Color c);
