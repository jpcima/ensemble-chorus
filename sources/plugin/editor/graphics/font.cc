//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "font.h"
#include "utility/stdc_memory.h"
#include <gzip/decompress.hpp>
#include <cstdlib>
#include <cstring>

FontCollection::FontCollection(NanoVG &g)
    : g_(g)
{
}

NanoVG::FontId FontCollection::getSansRegular()
{
    const uint8_t data[] = {
        #include "../fonts/Sans-Regular.h"
    };
    return getOrCreateFont("Sans Regular", data, sizeof(data), idSansRegular_);
}

NanoVG::FontId FontCollection::getSansBold()
{
    const uint8_t data[] = {
        #include "../fonts/Sans-Bold.h"
    };
    return getOrCreateFont("Sans Bold", data, sizeof(data), idSansBold_);
}

NanoVG::FontId FontCollection::getSansItalic()
{
    const uint8_t data[] = {
        #include "../fonts/Sans-Italic.h"
    };
    return getOrCreateFont("Sans Italic", data, sizeof(data), idSansItalic_);
}

NanoVG::FontId FontCollection::getSansBoldItalic()
{
    const uint8_t data[] = {
        #include "../fonts/Sans-BoldItalic.h"
    };
    return getOrCreateFont("Sans Bold Italic", data, sizeof(data), idSansBoldItalic_);
}

NanoVG::FontId FontCollection::getSerifRegular()
{
    const uint8_t data[] = {
        #include "../fonts/Serif-Regular.h"
    };
    return getOrCreateFont("Serif Regular", data, sizeof(data), idSerifRegular_);
}

NanoVG::FontId FontCollection::getSerifBold()
{
    const uint8_t data[] = {
        #include "../fonts/Serif-Bold.h"
    };
    return getOrCreateFont("Serif Bold", data, sizeof(data), idSerifBold_);
}

NanoVG::FontId FontCollection::getSerifItalic()
{
    const uint8_t data[] = {
        #include "../fonts/Serif-Italic.h"
    };
    return getOrCreateFont("Serif Italic", data, sizeof(data), idSerifItalic_);
}

NanoVG::FontId FontCollection::getSerifBoldItalic()
{
    const uint8_t data[] = {
        #include "../fonts/Serif-BoldItalic.h"
    };
    return getOrCreateFont("Serif Bold Italic", data, sizeof(data), idSerifBoldItalic_);
}

NanoVG::FontId FontCollection::getMonoRegular()
{
    const uint8_t data[] = {
        #include "../fonts/Mono-Regular.h"
    };
    return getOrCreateFont("Mono Regular", data, sizeof(data), idMonoRegular_);
}

NanoVG::FontId FontCollection::getMonoBold()
{
    const uint8_t data[] = {
        #include "../fonts/Mono-Bold.h"
    };
    return getOrCreateFont("Mono Bold", data, sizeof(data), idMonoBold_);
}

NanoVG::FontId FontCollection::getMonoItalic()
{
    const uint8_t data[] = {
        #include "../fonts/Mono-Italic.h"
    };
    return getOrCreateFont("Mono Italic", data, sizeof(data), idMonoItalic_);
}

NanoVG::FontId FontCollection::getMonoBoldItalic()
{
    const uint8_t data[] = {
        #include "../fonts/Mono-BoldItalic.h"
    };
    return getOrCreateFont("Mono Bold Italic", data, sizeof(data), idMonoBoldItalic_);
}

NanoVG::FontId FontCollection::getOrCreateFont(const char *name, const uint8_t *gzdata, size_t gzsize, NanoVG::FontId &id)
{
    if (id != -1)
        return id;
    NanoVG &g = g_;
    std::string fontdata = gzip::decompress((const char *)gzdata, gzsize);

    stdc_unique_ptr<uint8_t> fontmem(
        (uint8_t *)std::malloc(fontdata.size()));
    if (!fontmem)
        throw std::bad_alloc();
    std::memcpy(fontmem.get(), fontdata.data(), fontdata.size());

    id = g.createFontFromMemory(
        name, fontmem.get(), fontdata.size(), true);
    if (id != -1)
        fontmem.release();

    return id;
}
