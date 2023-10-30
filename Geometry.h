#include <QPoint>

#include <optional>

int findSector(const QPoint& p1, const QPoint& p2);

inline long dot2D(const QPoint& p1, const QPoint& p2)
{
    return p1.x() * p2.x() + p1.y() * p2.y();
}

inline long cross2D(const QPoint& p1, const QPoint& p2)
{
    return p1.x() * p2.y() - p1.y() * p2.x();
}

std::optional<QPoint> segmentIntersection(const QPoint& p1, const QPoint& p2, const QPoint& p3, const QPoint& p4);

std::optional<QPoint> lineIntersection(const QPoint& p1, const QPoint& p2, const QPoint& p3, const QPoint& p4);

QPair<QPoint, QPoint> offsetSegment(const QPoint& p1, const QPoint& p2, const int& offset);

bool polygonSign(const QVector<QPoint>& points);
