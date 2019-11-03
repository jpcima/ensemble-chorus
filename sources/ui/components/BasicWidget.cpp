#include "BasicWidget.hpp"

void BasicWidget::setBounds(const Rect &rect)
{
    setAbsolutePos(rect.x, rect.y);
    setSize(rect.w, rect.h);
}

Rect BasicWidget::getBounds() const
{
    return Rect{getAbsoluteX(), getAbsoluteY(), (int)getWidth(), (int)getHeight()};
}
