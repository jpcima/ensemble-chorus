//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "box.h"
#include "colors.h"
#include <cassert>

static constexpr float border_width = 2;

static void shadow_frame(NanoVG &g, float x, float y, float w, float h, Color c)
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

static void shadow_box(NanoVG &g, float x, float y, float w, float h, Color c)
{
    const float bw = 3;

    g.beginPath();
    g.rect(x + 1, y + 1, w - 2 - bw, h - 2 - bw);
    g.fillColor(c);
    g.fill();

    shadow_frame(g, x , y, w, h, Colors::gray0());
}

static const Color gray_ramp[24] = {
    Color(0x00, 0x00, 0x00),
    Color(0x0d, 0x0d, 0x0d),
    Color(0x1a, 0x1a, 0x1a),
    Color(0x26, 0x26, 0x26),
    Color(0x31, 0x31, 0x31),
    Color(0x3d, 0x3d, 0x3d),
    Color(0x48, 0x48, 0x48),
    Color(0x55, 0x55, 0x55),
    Color(0x5f, 0x5f, 0x5f),
    Color(0x6a, 0x6a, 0x6a),
    Color(0x75, 0x75, 0x75),
    Color(0x80, 0x80, 0x80),
    Color(0x8a, 0x8a, 0x8a),
    Color(0x95, 0x95, 0x95),
    Color(0xa0, 0xa0, 0xa0),
    Color(0xaa, 0xaa, 0xaa),
    Color(0xb5, 0xb5, 0xb5),
    Color(0xc0, 0xc0, 0xc0),
    Color(0xcb, 0xcb, 0xcb),
    Color(0xd5, 0xd5, 0xd5),
    Color(0xe0, 0xe0, 0xe0),
    Color(0xea, 0xea, 0xea),
    Color(0xf5, 0xf5, 0xf5),
    Color(0xff, 0xff, 0xff),
};

static void frame2(NanoVG &g, const char *s, int x, int y, int w, int h)
{
    if (!(h > 0 && w > 0))
        return;

    const Color *gr = gray_ramp - 'A';
    constexpr bool fill = true;
    constexpr float vlinewidth = 2;
    constexpr float hlinewidth = 1;

    while (*s) {
        // draw bottom line:
        g.beginPath();
        if (fill) {
            g.rect(x, y + h - 1, w, vlinewidth);
            g.fillColor(gr[(unsigned char)*s++]);
            g.fill();
        }
        else {
            g.moveTo(x, y + h - 1);
            g.lineTo(x + w - 1, y + h - 1);
            g.strokeColor(gr[(unsigned char)*s++]);
            g.stroke();
        }
        if (--h <= 0) break;
        // draw right line:
        g.beginPath();
        if (fill) {
            g.rect(x + w - 1, y, hlinewidth, h);
            g.fillColor(gr[(unsigned char)*s++]);
            g.fill();
        }
        else {
            g.moveTo(x + w - 1, y + h - 1);
            g.lineTo(x + w - 1, y);
            g.strokeColor(gr[(unsigned char)*s++]);
            g.stroke();
        }
        if (--w <= 0) break;
        // draw top line:
        g.beginPath();
        if (fill) {
            g.rect(x, y, w, vlinewidth);
            g.fillColor(gr[(unsigned char)*s++]);
            g.fill();
        }
        else {
            g.moveTo(x, y);
            g.lineTo(x + w - 1, y);
            g.strokeColor(gr[(unsigned char)*s++]);
            g.stroke();
        }
        y++; if (--h <= 0) break;
        // draw left line:
        g.beginPath();
        if (fill) {
            g.rect(x, y, hlinewidth, h);
            g.fillColor(gr[(unsigned char)*s++]);
            g.fill();
        }
        else {
            g.moveTo(x, y + h - 1);
            g.lineTo(x, y);
            g.strokeColor(gr[(unsigned char)*s++]);
            g.stroke();
        }
        x++; if (--w <= 0) break;
    }
}

static void down_frame(NanoVG &g, float x, float y, float w, float h, Color c)
{
    if (border_width == 1)
        frame2(g, "WWHH", x, y, w, h);
    else if (border_width == 2)
        frame2(g, "WWMMPPAA", x, y, w, h);
    else
        assert(false);  // frame(g, "NNTUJJWWAAAA", x, y, w, h);
}

static void down_box(NanoVG &g, float x, float y, float w, float h, Color c)
{
    down_frame(g, x, y, w, h, c);
    g.beginPath();
    g.rect(x + border_width, y + border_width, w - 2 * border_width, h - 2 * border_width);
    g.fillColor(c);
    g.fill();
}

static void up_frame(NanoVG &g, float x, float y, float w, float h, Color c)
{
    if (border_width == 1)
        frame2(g, "HHWW", x, y, w, h);
    else if (border_width == 2)
        frame2(g, "AAWWMMTT", x, y, w, h);
    else
        assert(false);  // frame(g, "AAAAWWJJUTNN", x, y, w, h);
}

static void up_box(NanoVG &g, float x, float y, float w, float h, Color c)
{
    up_frame(g, x, y, w, h, c);
    g.beginPath();
    g.rect(x + border_width, y + border_width, w - 2 * border_width, h - 2 * border_width);
    g.fillColor(c);
    g.fill();
}

static void thin_down_frame(NanoVG &g, float x, float y, float w, float h, Color)
{
    frame2(g, "WWHH", x, y, w, h);
}

static void thin_down_box(NanoVG &g, float x, float y, float w, float h, Color c)
{
    thin_down_frame(g, x, y, w, h, c);
    g.beginPath();
    g.rect(x + 1, y + 1, w - 2, h - 2);
    g.fillColor(c);
    g.fill();
}

static void thin_up_frame(NanoVG &g, float x, float y, float w, float h, Color)
{
    frame2(g, "HHWW", x, y, w, h);
}

static void thin_up_box(NanoVG &g, float x, float y, float w, float h, Color c)
{
    thin_up_frame(g, x, y, w, h, c);
    g.beginPath();
    g.rect(x + 1, y + 1, w - 2, h - 2);
    g.fillColor(c);
    g.fill();
}

void styled_box(NanoVG &g, Box_Style bs, float x, float y, float w, float h, Color c)
{
    if (bs == BS_NO_BOX)
        return;
    void (*boxfns[])(NanoVG &, float, float, float, float, Color) = {
        &shadow_box,
        &down_box,
        &up_box,
        &thin_down_box,
        &thin_up_box,
        &shadow_frame,
        &down_frame,
        &up_frame,
        &thin_down_frame,
        &thin_up_frame,
    };
    return boxfns[bs](g, x, y, w, h, c);
}

void styled_box_deltas(Box_Style bs, float *dx, float *dy, float *dw, float *dh)
{
    if (bs == BS_NO_BOX) {
        if (dx) *dx = 0;
        if (dy) *dy = 0;
        if (dw) *dw = 0;
        if (dh) *dh = 0;
    }
    constexpr float d1 = border_width;
    constexpr float d2 = 2 * d1;
    float deltas[][4] = {
        {1, 1, 5, 5},
        {d1, d1, d2, d2},
        {d1, d1, d2, d2},
        {1, 1, 2, 2},
        {1, 1, 2, 2},
        {1, 1, 5, 5},
        {d1, d1, d2, d2},
        {d1, d1, d2, d2},
        {1, 1, 2, 2},
        {1, 1, 2, 2},
    };
    if (dx) *dx = deltas[bs][0];
    if (dy) *dy = deltas[bs][1];
    if (dw) *dw = deltas[bs][2];
    if (dh) *dh = deltas[bs][3];
}
