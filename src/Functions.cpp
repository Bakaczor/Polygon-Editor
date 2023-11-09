#include <QtMath>

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
        case Algorithm::Enum::SymmetricDoubleStep:
        {
            drawSymmetricDoubleStep(painter, p1, p2, thicc, color);
            break;
        }
        case Algorithm::Enum::GuptaSproull:
        {
            drawGuptaSproull(painter, p1, p2, thicc, color);
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

void drawBack(QSharedPointer<QPainter> painter, const QVector<QPoint>& points, const QColor& color)
{
    QBrush b(color);
    b.setStyle(Qt::SolidPattern);
    painter->setBrush(b);
    QPen p(color);
    p.setWidth(1);
    painter->setPen(p);
    QPolygon poly(points);
    painter->drawPolygon(poly);
}

void drawSymmetricDoubleStep(QSharedPointer<QPainter> painter, const QPoint &p1, const QPoint &p2, const int& thicc, const QColor &color)
{
    int x1 = p1.x();
    int x2 = p2.x();
    int y1 = p1.x();
    int y2 = p2.y();

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

    int end = (dx - 1) >> 2;
    int n = (dx - 1) % 4 > 0 ? end : end - 1;

    putPixel(painter, x1, y1, thicc, color);
    putPixel(painter, x2, y2, thicc, color);

    if (dx > dy)
    {
        int de = dy << 2;
        int dne = de - (dx << 1);
        int d = de - dx;

        int dy2 = dy << 1;
        for (int i = 0; i < n; i++)
        {
            x1++;
            x2--;
            if (d < 0)
            {
                pattern1x(painter, x1, y1, xi, thicc, color);
                pattern1x(painter, x2, y2, -xi, thicc, color);
                d += de;
            }
            else if (d - dy2 < 0)
            {
                pattern2x(painter, x1, y1, xi, yi, thicc, color);
                pattern2x(painter, x2, y2, -xi, -yi, thicc, color);
                d += dne;
            }
            else
            {
                pattern3x(painter, x1, y1, xi, yi, thicc, color);
                pattern3x(painter, x2, y2, -xi, -yi, thicc, color);
                d += dne;
            }
        }
    }
    else
    {
        int dn = dx << 2;
        int dne = dn - (dy << 1);
        int d = dn - dy;

        int dx2 = dx << 1;
        for (int i = 0; i < n; i++)
        {
            y1++;
            y2--;
            if (d < 0)
            {
                pattern1y(painter, y1, x1, yi, thicc, color);
                pattern1y(painter, y2, x2, -yi, thicc, color);
                d += dn;
            }
            else if (d - dx2 < 0)
            {
                pattern2y(painter, y1, x1, yi, xi, thicc, color);
                pattern2y(painter, y2, x2, -yi, -xi, thicc, color);
                d += dne;
            }
            else
            {
                pattern3y(painter, y1, x1, yi, xi, thicc, color);
                pattern3y(painter, y2, x2, -yi, -xi, thicc, color);
                d += dne;
            }
        }
    }
}

void drawGuptaSproull(QSharedPointer<QPainter> painter, const QPoint &p1, const QPoint &p2, const int &thicc, const QColor &color)
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

    float invDenom = 1 / qSqrt(dx * dx + dy * dy);
    if (dx > dy)
    {
        int dne = (dy - dx) << 1;
        int de = dy << 1;
        int d = de - dx;
        int two_v_dx = 0;
        float two_dx_invDenom = 2 * dx * invDenom;

        int i;
        IntensifyPixel(painter, x, y, thicc, 0, color);
        for (i = 1; IntensifyPixel(painter, x, y + i, thicc, i * two_dx_invDenom, color); i++);
        for (i = 1; IntensifyPixel(painter, x, y - i, thicc, i * two_dx_invDenom, color); i++);
        while (x != p2.x())
        {
            x += xi;
            if (d >= 0)
            {
                y += yi;
                two_v_dx = d - dx;
                d += dne;
            }
            else
            {
                two_v_dx = d + dx;
                d += de;
            }
            IntensifyPixel(painter, x, y, thicc, two_v_dx * invDenom, color);
            for (i = 1; IntensifyPixel(painter, x, y + i, thicc, i * two_dx_invDenom - two_v_dx * invDenom, color); i++);
            for (i = 1; IntensifyPixel(painter, x, y - i, thicc, i * two_dx_invDenom + two_v_dx * invDenom, color); i++);
        }
    }
    else
    {
        int dne = (dx - dy) << 1;
        int dn = dx << 1;
        int d = dn - dy;
        int two_v_dy = 0;
        float two_dy_invDenom = 2 * dy * invDenom;

        int i;
        IntensifyPixel(painter, x, y, thicc, 0, color);
        for (i = 1; IntensifyPixel(painter, x + i, y, thicc, i * two_dy_invDenom, color); i++);
        for (i = 1; IntensifyPixel(painter, x - i, y, thicc, i * two_dy_invDenom, color); i++);
        while (y != p2.y())
        {
            y += yi;
            if (d >= 0)
            {
                x += xi;
                two_v_dy = d - dy;
                d += dne;
            }
            else
            {
                two_v_dy = d + dy;
                d += dn;
            }
            IntensifyPixel(painter, x, y, thicc, two_v_dy * invDenom, color);
            for (i = 1; IntensifyPixel(painter, x + i, y, thicc, i * two_dy_invDenom - two_v_dy * invDenom, color); i++);
            for (i = 1; IntensifyPixel(painter, x - i, y, thicc, i * two_dy_invDenom + two_v_dy * invDenom, color); i++);
        }
    }
}
