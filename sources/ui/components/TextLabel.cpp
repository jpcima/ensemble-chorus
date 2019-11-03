#include "TextLabel.hpp"
#include "Window.hpp"
#include "Cairo.hpp"

TextLabel::TextLabel(FontEngine &fe, Widget *group)
    : BasicWidget(group), fFontEngine(fe)
{
}

void TextLabel::setText(std::string text)
{
    if (fText == text)
        return;

    fText = text;
    repaint();
}

void TextLabel::setTextFont(const Font &font)
{
    if (fTextFont == font)
        return;

    fTextFont = font;
    repaint();
}

void TextLabel::onDisplay()
{
    cairo_t *cr = getParentWindow().getGraphicsContext().cairo;

    int w = getWidth();
    int h = getHeight();

    //
    const std::string &text = fText;
    if (!text.empty())
        fFontEngine.drawInBox(cr, text.c_str(), fTextFont, RectF(fTextOffset.x, fTextOffset.y, w, h), fTextAlign);
}
