#include <QPainter>

#include "Enums.h"

void drawLine(QSharedPointer<QPainter> painter, const QPoint& p1, const QPoint& p2,
                     const Algorithm& type, const int& thicc, const QColor& color);
