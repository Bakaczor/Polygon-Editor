#include "Functions.h"

void drawLine(QSharedPointer<QPainter> painter, const QPoint& p1, const QPoint& p2,
              const Algorithm& type, const int& thicc, const QColor& color)
{
    QPen p;
    p.setWidth(thicc);
    p.setColor(color);
    painter->setPen(p);
    
    if (type == Algorithm::Library)
    {
        painter->drawLine(p1, p2);
    }
}
