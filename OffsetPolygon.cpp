#include <QStack>
#include <vector>
#include <algorithm>

#include "Functions.h"
#include "Geometry.h"
#include "OffsetPolygon.h"

OffsetPolygon::OffsetPolygon(const QVector<QPoint> &points, const int &offset)
{
    m_polygon = QVector<QPoint>();
    m_loops = QVector<QVector<QPoint>>();
    if (offset != 0 && !points.empty())
    {
        update(points, offset);
    }
}

void OffsetPolygon::paint(QSharedPointer<QPainter> painter, const Algorithm::Enum &type) const
{
    //drawBack(painter, m_polygon, s_color);
    for (uint i = 0; i < m_polygon.count() - 1; i++)
    {
       drawLine(painter, m_polygon.at(i), m_polygon.at(i + 1), type, s_thicc, s_color);
    }
    drawLine(painter, m_polygon.constLast(), m_polygon.constFirst(), type, s_thicc, s_color);
    for (const QVector<QPoint>& poly : m_loops)
    {
        //drawBack(painter, poly, QColor(255, 255, 255, 255));
        for (uint i = 0; i < poly.count() - 1; i++)
        {
            drawLine(painter, poly.at(i), poly.at(i + 1), type, s_thicc, s_color);
        }
        drawLine(painter, poly.constLast(), poly.constFirst(), type, s_thicc, s_color);
    }
}

void OffsetPolygon::update(const QVector<QPoint>& points, const int &offset)
{
    m_polygon.clear();
    m_loops.clear();
    if (offset == 0) { return; }
    QVector<QPoint> baseIntersections;
    if (offset <= s_pointError)
    {
        baseIntersections = points;
    }
    else
    {
        baseIntersections = calculateIntersections(points, offset);
    }
    QVector<QPair<QPoint, bool>> allIntersections = addSelfIntersections(baseIntersections);
    m_loops = removeNeedlessAndGetLoops(allIntersections);
    m_polygon = removeDoublesAndCollinear(allIntersections);
}

QVector<QPoint> calculateIntersections(const QVector<QPoint>& points, const int& offset)
{
    QVector<QPair<QPoint, QPoint>> segments;
    segments.reserve(points.count());
    for (uint i = 0; i < points.count() - 1; i++)
    {
        segments.append(offsetSegment(points.at(i), points.at(i + 1), offset));
    }
    segments.append(offsetSegment(points.constLast(), points.constFirst(), offset));

    QVector<QPoint> baseIntersections;
    baseIntersections.reserve(segments.count());
    std::optional<QPoint> opt = lineIntersection(segments.constLast().first, segments.constLast().second, segments.constFirst().first, segments.constFirst().second);
    if (opt.has_value())
    {
        baseIntersections.append(opt.value());
    }
    for (uint i = 0; i < segments.count() - 1; i++)
    {
        std::optional<QPoint> opt = lineIntersection(segments.at(i).first, segments.at(i).second, segments.at(i + 1).first, segments.at(i + 1).second);
        if (opt.has_value())
        {
            baseIntersections.append(opt.value());
        }
    }
    return baseIntersections;
}

QVector<QPair<QPoint, bool>> addSelfIntersections(const QVector<QPoint>& baseIntersections)
{
    uint n = baseIntersections.count();
    QVector<QPair<QPoint, bool>> allIntersections;
    allIntersections.reserve(n);
    for (uint i = 0; i < n; i++)
    {
        const QPoint& iP = baseIntersections.at(i);
        uint ni = (i + 1) % n;
        // move the base intersection
        allIntersections.append(qMakePair(iP, false));

        std::vector<QPoint> intersections;
        for (uint j = 0; j < n; j++)
        {
            uint nj = (j + 1) % n;
            // skip obvious intersections
            if (i == j || i == nj || ni == j) { continue; }
            std::optional<QPoint> opt = segmentIntersection(iP, baseIntersections.at(ni), baseIntersections.at(j), baseIntersections.at(nj));
            if (opt.has_value())
            {
                intersections.push_back(opt.value());
            }
        }

        // sort new intersections by the distance to iP
        std::sort(begin(intersections), end(intersections), [&iP](const QPoint& p1, const QPoint& p2) {
            const int dx1 = p1.x() - iP.x();
            const int dy1 = p1.y() - iP.y();
            const int dx2 = p2.x() - iP.x();
            const int dy2 = p2.y() - iP.y();
            return dx1 * dx1 + dy1 * dy1 < dx2 * dx2 + dy2 * dy2;
        });

        for (const QPoint& p : intersections)
        {
            allIntersections.append(qMakePair(p, true));
        }
    }
    return allIntersections;
}

QVector<QVector<QPoint>> removeNeedlessAndGetLoops(QVector<QPair<QPoint, bool>>& allIntersections)
{
    const auto equal = [](const QPoint& p1, const QPoint& p2) {
        return qFabs(p1.x() - p2.x()) < OffsetPolygon::s_pointError && qFabs(p1.y() - p2.y()) < OffsetPolygon::s_pointError;
    };
    uint n = allIntersections.count();
    QVector<QVector<QPoint>> loops;
    QVector<bool> toTake(n, true);
    QStack<QPair<QPoint, uint>> stack;
    bool flag = true;
    uint i = 0;
    for (const QPair<QPoint, bool>& pair : allIntersections)
    {
        if (pair.second)
        {
            flag = !flag;
            if (stack.empty())
            {
                stack.push(qMakePair(pair.first, i));
            }
            else
            {
                const auto& top = stack.top();
                if (equal(top.first, pair.first))
                {
                    // calculate number of points in loop
                    uint taken = 0;
                    for (uint j = top.second + 1; j < i; j++)
                    {
                        if (toTake[j])
                        {
                            taken++;
                        }
                    }
                    if (taken >= 2)
                    {
                        QVector<QPoint> loop;
                        for (uint j = top.second; j < i; j++)
                        {
                            if (toTake[j])
                            {
                                loop.append(allIntersections.at(j).first);
                                toTake[j] = false;
                            }
                        }
                        loops.append(loop);

                        // check the collinearity just after the loop
                        const QPoint& iP = pair.first;
                        const QPoint& niP = allIntersections.at((i + 1) % n).first;
                        const QPoint& nniP = allIntersections.at((i + 2) % n).first;
                        if (qFabs(cross2D(iP - niP, nniP - niP)) < OffsetPolygon::s_crossError)
                        {
                            toTake[(i + 1) % n] = false;
                        }
                    }
                    toTake[i] = false;
                    stack.pop();
                }
                else
                {
                    if (toTake[i])
                    {
                        stack.push(qMakePair(pair.first, i));
                    }
                }
            }
        }
        else
        {
            if (!flag)
            {
                toTake[i] = false;
            }
        }
        i++;
    }
    QVector<QPair<QPoint, bool>> taken;
    for (i = 0; i < toTake.count(); i++)
    {
        if (toTake.at(i))
        {
            taken.append(allIntersections.at(i));
        }
    }
    allIntersections = taken;
    return loops;
}

QVector<QPoint> removeDoublesAndCollinear(const QVector<QPair<QPoint, bool>>& allIntersections)
{
    const auto equal = [](const QPoint& p1, const QPoint& p2) {
        return qFabs(p1.x() - p2.x()) < OffsetPolygon::s_pointError && qFabs(p1.y() - p2.y()) < OffsetPolygon::s_pointError;
    };

    QVector<QPoint> unique;
    uint n = allIntersections.count();
    uint i = 0;
    while (i < n)
    {
        unique.append(allIntersections.at(i).first);
        uint ni = (i + 1) % n;
        if (equal(allIntersections.at(i).first, allIntersections.at(ni).first))
        {
            // skip double
            i += 2;
        }
        else
        {
            i += 1;
        }
    }

    QVector<QPoint> mainLoop;
    n = unique.count();
    i = 0;
    while (i < n)
    {
        mainLoop.append(unique.at(i));
        uint ni = (i + 1) % n;
        uint nni = (i + 2) % n;
        const QPoint p2p1 = unique.at(i) - unique.at(ni);
        const QPoint p2p3 = unique.at(nni) - unique.at(ni);
        if (qFabs(cross2D(p2p1, p2p3)) < OffsetPolygon::s_crossError)
        {
            // skip collinear
            i += 2;
        }
        else
        {
            i += 1;
        }
    }
    return mainLoop;
}
