#include <QPainter>
#include <QSharedPointer>

#include "Enums.h"

void putPixel(QSharedPointer<QPainter> painter, const int& x, const int& y, const int& thicc, const QColor& color);

void drawBack(QSharedPointer<QPainter> painter, const QVector<QPoint>& points, const QColor& color);

void drawLine(QSharedPointer<QPainter> painter, const QPoint& p1, const QPoint& p2, const Algorithm::Enum& type, const int& thicc, const QColor& color);

void drawLibrary(QSharedPointer<QPainter> painter, const QPoint& p1, const QPoint& p2, const int& thicc, const QColor& color);

void drawBresenham(QSharedPointer<QPainter> painter, const QPoint& p1, const QPoint& p2, const int& thicc, const QColor& color);
