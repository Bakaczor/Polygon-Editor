#include <QPainter>

#include "Icon.h"

Icon::Icon(QQuickItem* parent) : QQuickPaintedItem(parent), m_source("") {
    setWidth(15);
    setHeight(20);
}

void Icon::paint(QPainter* painter) {
    painter->drawImage(QRect(position().x(), position().y(), width(), height()), QImage(m_source));
}

void Icon::setSource(Orientation::Enum type)
{
    switch(type)
    {
        case Orientation::Enum::Horizontal:
        {
            m_source = ":/h.png";
            break;
        }
        case Orientation::Enum::Vertical:
        {
            m_source = ":/v.png";
            break;
        }
        case Orientation::Enum::None:
        {
            m_source = "";
            break;
        }
    }
}
