//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "box.h"
#include "colors.h"

void shadow_frame(NanoVG &g, float x, float y, float w, float h, Color c)
{
    const float bw = 3;

    g.fillColor(Colors::dark3());
    g.beginPath();
    g.rect(x + bw, y + h - bw,  w - bw, bw);
    g.fill();
    g.beginPath();
    g.rect(x + w - bw,  y + bw, bw,  h - bw);
    g.fill();

    g.strokeColor(c);
    g.beginPath();
    g.rect(x, y, w - bw, h - bw);
    g.stroke();
}

void shadow_box(NanoVG &g, float x, float y, float w, float h, Color c)
{
    const float bw = 3;

    g.beginPath();
    g.rect(x + 1, y + 1, w - 2 - bw, h - 2 - bw);
    g.fillColor(c);
    g.fill();

    shadow_frame(g, x , y, w, h, Colors::gray0());
}
