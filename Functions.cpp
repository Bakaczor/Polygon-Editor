#include "Functions.h"

void drawLine(QSharedPointer<QPainter> painter, const QPoint& p1, const QPoint& p2, enum Algorithm type)
{
    QPen p;
    p.setWidth(2);
    painter->setPen(p);
    
    if (type == Algorithm::Library)
    {
        painter->drawLine(p1, p2);
    }
}
