//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dgl/NanoVG.hpp>

void shadow_text(NanoVG &g, float x, float y, float w, const char *text, Color c);
void engraved_text(NanoVG &g, float x, float y, float w, const char *text, Color c);
void embossed_text(NanoVG &g, float x, float y, float w, const char *text, Color c);

void multilayer_text(
    NanoVG &g, float x, float y, float w, const char *text,
    const float offs[][2], const Color cols[], unsigned n);
