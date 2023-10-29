#include "Functions.h"

void putPixel(QSharedPointer<QPainter> painter, const int& x, const int& y, const int& thicc, const QColor& color)
{
    QPen p;
    p.setWidth(thicc);
    p.setColor(color);
    painter->setPen(p);
    painter->drawPoint(x, y);
}

void drawLine(QSharedPointer<QPainter> painter, const QPoint& p1, const QPoint& p2, const Algorithm::Enum& type, const int& thicc, const QColor& color)
{
    switch(type)
    {
        case Algorithm::Enum::Library:
        {
            drawLibrary(painter, p1, p2, thicc, color);
            break;
        }
        case Algorithm::Enum::Bresenham:
        {
            drawBresenham(painter, p1, p2, thicc, color);
            break;
        }

    }
}

void drawLibrary(QSharedPointer<QPainter> painter, const QPoint &p1, const QPoint &p2, const int &thicc, const QColor &color)
{
    QPen p;
    p.setWidth(thicc);
    p.setColor(color);
    painter->setPen(p);
    painter->drawLine(p1, p2);
}

void drawBresenham(QSharedPointer<QPainter> painter, const QPoint &p1, const QPoint &p2, const int& thicc, const QColor &color)
{
    int x = p1.x(), y = p1.y();

    int dx, xi;
    if (p1.x() < p2.x())
    {
            xi = 1;
            dx = p2.x() - p1.x();
    }
    else
    {
            xi = -1;
            dx = p1.x() - p2.x();
    }

    int dy, yi;
    if (p1.y() < p2.y())
    {
            yi = 1;
            dy = p2.y() - p1.y();
    }
    else
    {
            yi = -1;
            dy = p1.y() - p2.y();
    }

    putPixel(painter, x, y, thicc, color);

    if (dx > dy)
    {
            int dne = (dy - dx) << 1;
            int de = dy << 1;
            int d = de - dx;
            while (x != p2.x())
            {
                x += xi;
                if (d >= 0)
                {
                    y += yi;
                    d += dne;
                }
                else
                {
                    d += de;
                }
                putPixel(painter, x, y, thicc, color);
            }
    }
    else
    {
            int dne = (dx - dy) << 1;
            int dn = dx << 1;
            int d = dn - dy;
            while (y != p2.y())
            {
                y += yi;
                if (d >= 0)
                {
                    x += xi;
                    d += dne;
                }
                else
                {
                    d += dn;
                }
                putPixel(painter, x, y, thicc, color);
            }
    }
}
