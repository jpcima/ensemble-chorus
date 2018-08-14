//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <dgl/NanoVG.hpp>

namespace Colors {
    inline Color gray0() { return Color(0x00, 0x00, 0x00); }

    inline Color dark1() { return Color(0xaa, 0xaa, 0xaa); }
    inline Color dark2() { return Color(0x95, 0x95, 0x95); }
    inline Color dark3() { return Color(0x55, 0x55, 0x55); }

    inline Color light1() { return Color(0xcb, 0xcb, 0xcb); }
    inline Color light2() { return Color(0xe0, 0xe0, 0xe0); }
    inline Color light3() { return Color(0xf5, 0xf5, 0xf5); }

    inline Color white() { return Color(0xff, 0xff, 0xff); }
};
