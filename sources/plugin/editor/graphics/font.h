//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <dgl/NanoVG.hpp>

class FontCollection {
public:
    explicit FontCollection(NanoVG &g);

    NanoVG::FontId getSansRegular();
    NanoVG::FontId getSansBold();
    NanoVG::FontId getSansItalic();
    NanoVG::FontId getSansBoldItalic();
    NanoVG::FontId getSerifRegular();
    NanoVG::FontId getSerifBold();
    NanoVG::FontId getSerifItalic();
    NanoVG::FontId getSerifBoldItalic();
    NanoVG::FontId getMonoRegular();
    NanoVG::FontId getMonoBold();
    NanoVG::FontId getMonoItalic();
    NanoVG::FontId getMonoBoldItalic();

private:
    NanoVG &g_;

    NanoVG::FontId idSansRegular_ = -1;
    NanoVG::FontId idSansBold_ = -1;
    NanoVG::FontId idSansItalic_ = -1;
    NanoVG::FontId idSansBoldItalic_ = -1;
    NanoVG::FontId idSerifRegular_ = -1;
    NanoVG::FontId idSerifBold_ = -1;
    NanoVG::FontId idSerifItalic_ = -1;
    NanoVG::FontId idSerifBoldItalic_ = -1;
    NanoVG::FontId idMonoRegular_ = -1;
    NanoVG::FontId idMonoBold_ = -1;
    NanoVG::FontId idMonoItalic_ = -1;
    NanoVG::FontId idMonoBoldItalic_ = -1;

    NanoVG::FontId getOrCreateFont(const char *name, const uint8_t *gzdata, size_t gzsize, NanoVG::FontId &id);
};
