#include <QVector>
#include <QtMath>

#include "Geometry.h"

int findSector(const QPoint &p1, const QPoint &p2)
{
    const int dx = p2.x() - p1.x();
    const int dy = p2.y() - p1.y();
    const int mdx = dx >= 0 ? dx : -dx;
    const int mdy = dy >= 0 ? dy : -dy;

    if (dx >= 0)
    {
        if (dy >= 0)
        {
            if (5 * mdy < 2 * mdx) return 0;
            else if (2 * mdy > 5 * mdx) return 2;
            else return 1;
        }
        else
        {
            if (-5 * mdy > 2 * mdx) return 0;
            else if (-2 * mdy < 5 * mdx) return 6;
            else return 7;
        }
    }
    else
    {
        if (dy >= 0)
        {
            if (-5 * mdy > 2 * mdx) return 4;
            else if (-2 * mdy < 5 * mdx) return 2;
            else return 3;
        }
        else
        {
            if (5 * mdy < 2 * mdx) return 4;
            else if (2 * mdy > 5 * mdx) return 6;
            else return 5;
        }
    }
}

// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
std::optional<QPoint> segmentIntersection(const QPoint& p1, const QPoint& p2, const QPoint& p3, const QPoint& p4)
{
    const QPoint p = p1;
    const QPoint r = p2 - p1;
    const QPoint q = p3;
    const QPoint s = p4 - p3;

    const long r_s = cross2D(r, s);
    if (r_s == 0) { return std::nullopt; }

    const QPoint q_p = q - p;
    const float t = cross2D(q_p, s) / static_cast<float>(r_s);
    const float u = cross2D(q_p, r) / static_cast<float>(r_s);

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
        return p + QPoint(t * r.x(), t * r.y());
    }
    return std::nullopt;
}

std::optional<QPoint> lineIntersection(const QPoint& p1, const QPoint& p2, const QPoint& p3, const QPoint& p4)
{
    // A = dy
    // B = -dx
    // C = A * p.x + B * p.y
    const int A1 = p2.y() - p1.y();
    const int B1 = p1.x() - p2.x();
    const long C1 = A1 * p1.x() + B1 * p1.y();

    const int A2 = p4.y() - p3.y();
    const int B2 = p3.x() - p4.x();
    const long C2 = A2 * p3.x() + B2 * p3.y();

    const long det = cross2D(QPoint(A1, B1), QPoint(A2, B2));
    if (det == 0)
    {
        // if lines are parallel
        return std::nullopt;
    }
    else
    {
        // Crammer's rule
        int x = cross2D(QPoint(C1, B1), QPoint(C2, B2)) / static_cast<float>(det);
        int y = cross2D(QPoint(A1, C1), QPoint(A2, C2)) / static_cast<float>(det);
        return QPoint(x, y);
    }
}

QPair<QPoint, QPoint> offsetSegment(const QPoint& p1, const QPoint& p2, const int& offset)
{
    const QPoint A = p2 - p1 == QPoint(0, 0) ? QPoint(1, 1) : p2 - p1;
    QPoint P(A.y(), -A.x());
    const float len = qSqrt(P.x() * P.x() + P.y() * P.y());
    P.rx() = (offset * P.x()) / len;
    P.ry() = (offset * P.y()) / len;
    return QPair<QPoint, QPoint>(p1 + P, p2 + P);
}

bool polygonSign(const QVector<QPoint>& points)
{
    /*
    long long sum = 0;
    for (uint i = 0; i < points.count() - 1; i++)
    {
        sum += cross2D(points.at(i), points.at(i + 1));
    }
    sum += cross2D(points.constLast(), points.constFirst());
    return sum > 0;
    */

    uint n = points.count();
    uint i = 0;
    for (uint j = 1; j < n; j++)
    {
        if (points.at(j).x() <= points.at(i).x())
        {
            if (points.at(j).x() < points.at(i).x() || points.at(j).y() < points.at(i).y())
            {
                i = j;
            }
        }
    }
    const QPoint& A = points.at((i - 1) % n);
    const QPoint& B = points.at(i);
    const QPoint& C = points.at((i + 1) % n);
    return (B.x() - A.x()) * (C.y() - A.y()) - (C.x() - A.x()) * (B.y() - A.y()) > 0;
}
