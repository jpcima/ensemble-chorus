#pragma once
#include "BasicWidget.hpp"
#include "ui/FontEngine.h"
#include "ui/Color.h"
#include "ui/Geometry.h"
#include <string>
#include <functional>

class TextLabel : public BasicWidget {
public:
    TextLabel(FontEngine &fe, Widget *group);

    //
    const std::string &text() const { return fText; }
    void setText(std::string text);

    const Font &textFont() const { return fTextFont; }
    void setTextFont(const Font &font);

    int textAlign() const { return fTextAlign; }
    void setTextAlign(int align) { fTextAlign = align; }

    PointF textOffset() const { return fTextOffset; }
    void setTextOffset(PointF off) { fTextOffset = off; }

    //
    void onDisplay() override;

private:
    FontEngine &fFontEngine;
    std::string fText;
    Font fTextFont;
    ColorRGBA8 fTextColor;
    int fTextAlign = 0;
    PointF fTextOffset;
};
