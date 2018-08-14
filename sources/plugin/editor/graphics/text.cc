//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "text.h"
#include "colors.h"

void shadow_text(NanoVG &g, float x, float y, float w, const char *text, Color c)
{
    static const float offs[2][2] = {
        {2, 2}, {0, 0}
    };
    Color cols[2] = {
        Colors::dark3(), c
    };
    multilayer_text(g, x, y, w, text, offs, cols, 2);
}

void engraved_text(NanoVG &g, float x, float y, float w, const char *text, Color c)
{
    static const float offs[7][2] = {
        {1, 0}, {1, 1}, {0, 1},
        {-1, 0}, {-1, -1}, {0, -1},
        {0, 0}
    };
    Color cols[7] = {
        Colors::light3(), Colors::light3(), Colors::light3(),
        Colors::dark3(), Colors::dark3(), Colors::dark3(),
        c
    };
    multilayer_text(g, x, y, w, text, offs, cols, 7);
}

void embossed_text(NanoVG &g, float x, float y, float w, const char *text, Color c)
{
    static const float offs[7][2] = {
        {-1, 0}, {-1, -1}, {0, -1},
        {1, 0}, {1, 1}, {0, 1},
        {0,0}
    };
    Color cols[7] = {
        Colors::light3(), Colors::light3(), Colors::light3(),
        Colors::dark3(), Colors::dark3(), Colors::dark3(),
        c
    };
    multilayer_text(g, x, y, w, text, offs, cols, 7);
}

void multilayer_text(
    NanoVG &g, float x, float y, float w, const char *text,
    const float offs[][2], const Color cols[], unsigned n)
{
    for (unsigned i = 0; i < n; i++) {
        g.beginPath();
        g.fillColor(cols[i]);
        g.textBox(x + offs[i][0], y + offs[i][1], w, text);
    }
}
