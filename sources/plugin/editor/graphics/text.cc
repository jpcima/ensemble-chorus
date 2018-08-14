//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "text.h"
#include "colors.h"

void bounded_text(NanoVG &g, Text_Style ts, float x, float y, float w, float h, int align, const char *text, Color c)
{
    if (align & NanoVG::ALIGN_MIDDLE)
        y += 0.5f * h;
    else if (align & NanoVG::ALIGN_BOTTOM)
        y += h;
    g.textAlign(align);
    styled_text(g, ts, x, y, w, text, c);
}

static void multilayer_text(
    NanoVG &g, float x, float y, float w, const char *text,
    const float offs[][2], const Color cols[], unsigned n)
{
    for (unsigned i = 0; i < n; i++) {
        g.beginPath();
        g.fillColor(cols[i]);
        g.textBox(x + offs[i][0], y + offs[i][1], w, text);
    }
}

static void normal_text(NanoVG &g, float x, float y, float w, const char *text, Color c)
{
    g.beginPath();
    g.fillColor(c);
    g.textBox(x, y, w, text);
}

static void shadow_text(NanoVG &g, float x, float y, float w, const char *text, Color c)
{
    static const float offs[2][2] = {
        {2, 2}, {0, 0}
    };
    Color cols[2] = {
        Colors::dark3(), c
    };
    multilayer_text(g, x, y, w, text, offs, cols, 2);
}

static void engraved_text(NanoVG &g, float x, float y, float w, const char *text, Color c)
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

static void embossed_text(NanoVG &g, float x, float y, float w, const char *text, Color c)
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

void styled_text(NanoVG &g, Text_Style ts, float x, float y, float w, const char *text, Color c)
{
    void (*textfns[])(NanoVG &, float, float, float, const char *, Color) =
        {&normal_text, &shadow_text, &engraved_text, &embossed_text};
    return textfns[ts](g, x, y, w, text, c);
}
