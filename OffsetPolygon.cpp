#include <QStack>

#include "Functions.h"
#include "Geometry.h"
#include "OffsetPolygon.h"
#include "qdebug.h"

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

void OffsetPolygon::update(const QVector<QPoint> &points, const int &offset)
{
    m_polygon.clear();
    m_loops.clear();
    if (offset == 0) { return; }
    QVector<QPair<QPoint, QPoint>> segments = generateOffsetSegments(points, offset);
    /*
    QVector<QPoint> baseIntersections;
    for (const QPair<QPoint, QPoint>& seg : segments)
    {
        baseIntersections.append(seg.first);
        baseIntersections.append(seg.second);
    }
    */
    QVector<QPoint> baseIntersections = calculateBaseIntersections(segments);
    QVector<QPair<QPoint, bool>> allIntersections = calculateAllIntersections(baseIntersections);
    m_loops = removeNeedlessAndGetLoops(allIntersections);
    m_polygon = removeCollinearAndDoubles(allIntersections);
}

QVector<QPair<QPoint, QPoint> > generateOffsetSegments(const QVector<QPoint> &points, const int& offset)
{
    QVector<QPair<QPoint, QPoint>> segments;
    segments.reserve(points.count());
    for (uint i = 0; i < points.count() - 1; i++)
    {
        segments.append(offsetSegment(points.at(i), points.at(i + 1), offset));
    }
    segments.append(offsetSegment(points.constLast(), points.constFirst(), offset));
    /*
    qDebug() << "S";
    for(auto el : segments)
    {
        qDebug() << el << "\n";
    }
    */
    return segments;
}

QVector<QPoint> calculateBaseIntersections(const QVector<QPair<QPoint, QPoint> > &segments)
{
    QVector<QPoint> baseIntersections;
    baseIntersections.reserve(segments.count());
    for (uint i = 0; i < segments.count() - 1; i++)
    {
        std::optional<QPoint> opt = lineIntersection(segments.at(i).first, segments.at(i).second, segments.at(i + 1).first, segments.at(i + 1).second);
        if (opt.has_value())
        {
            baseIntersections.append(opt.value());
        }
    }
    std::optional<QPoint> opt = lineIntersection(segments.constLast().first, segments.constLast().second, segments.constFirst().first, segments.constFirst().second);
    if (opt.has_value())
    {
        baseIntersections.append(opt.value());
    }
    /*
    qDebug() << "BI";
    for(auto el : baseIntersections)
    {
        qDebug() << el;
    }
    qDebug() << "\n";
    */
    return baseIntersections;
}

QVector<QPair<QPoint, bool> > calculateAllIntersections(const QVector<QPoint> &baseIntersections)
{
    uint n = baseIntersections.count();
    QVector<QPair<QPoint, bool>> allIntersections;
    allIntersections.reserve(n);
    for (uint i = 0; i < n; i++)
    {
        uint ni = (i + 1) % n;
        allIntersections.append(qMakePair(baseIntersections.at(i), false));

        // dodać do pośredniej i posortować po odległości od baseIntersection[i]
        for (uint j = 0; j < n; j++)
        {
            uint nj = (j + 1) % n;
            // skip obvious intersections
            if (i == j || i == nj || ni == j) { continue; }
            std::optional<QPoint> opt = segmentIntersection(baseIntersections.at(i), baseIntersections.at(ni), baseIntersections.at(j), baseIntersections.at(nj));
            if (opt.has_value())
            {
                allIntersections.append(qMakePair(opt.value(), true));
            }
        }
    }
    qDebug() << "AI";
    /*
    for(auto el : allIntersections)
    {
        qDebug() << el;
    }
    qDebug() << "\n";
    */
    return allIntersections;
}

QVector<QVector<QPoint>> removeNeedlessAndGetLoops(QVector<QPair<QPoint, bool>> &allIntersections)
{
    QVector<QVector<QPoint>> loops;
    QVector<bool> toTake(allIntersections.count(), false);
    QStack<QPair<QPoint, uint>> stack;
    bool flag = true;
    uint i = 0;
    for (const QPair<QPoint, bool>& pair : allIntersections)
    {
        if (pair.second)
        {
            flag = !flag;
            toTake[i] = true;
            if (stack.empty())
            {
                stack.push(qMakePair(pair.first, i));
            }
            else
            {
                const auto& top = stack.top();
                if (qFabs(top.first.x() - pair.first.x()) < OffsetPolygon::s_margin &&
                    qFabs(top.first.y() - pair.first.y()) < OffsetPolygon::s_margin)
                {
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
                            loop.append(allIntersections.at(j).first);
                            toTake[j] = false;
                        }
                        toTake[i] = false;
                        loops.append(loop);
                        //sprawdzić cross2D trzech kolejnych, ale flaga musi zostać
                    }
                    // else toTake false
                    stack.pop();
                }
                else
                {
                    stack.push(qMakePair(pair.first, i));
                }
            }
        }
        else
        {
            if (flag)
            {
                toTake[i] = true;
            }
            else
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
    qDebug() << "L" << loops.count();
    for(auto& el : loops)
    {
        for(auto ell : el)
        {
            qDebug() << el;
        }
        qDebug() << "\n";
    }
    qDebug() << "\n";
    return loops;
}

QVector<QPoint> removeCollinearAndDoubles(const QVector<QPair<QPoint, bool>>& allIntersections)
{
    QVector<QPoint> mainLoop;
    uint n = allIntersections.count();
    uint i = 0;
    //  najpierw usuń duble, potem trójki cross2D
    while (i < n)
    {
        if (allIntersections.at(i).second)
        {
            uint ni = (i + 1) % n;
            if (qFabs(allIntersections.at(i).first.x() - allIntersections.at(ni).first.x()) < OffsetPolygon::s_margin &&
                qFabs(allIntersections.at(i).first.y() - allIntersections.at(ni).first.y()) < OffsetPolygon::s_margin)
            {
                mainLoop.append(allIntersections.at(i).first);
                i += 2;
            }
            else
            {
                uint nni = (i + 2) % n;
                const QPoint BA = allIntersections.at(i).first - allIntersections.at(ni).first;
                const QPoint BC = allIntersections.at(nni).first - allIntersections.at(ni).first;
                if (cross2D(BA, BC) != 0)
                {
                    mainLoop.append(allIntersections.at(i).first);
                    mainLoop.append(allIntersections.at(ni).first);
                }
                i += 2;
            }
        }
        else
        {
            mainLoop.append(allIntersections.at(i).first);
            i += 1;
        }
    }
    qDebug() << "ML";
    for(auto el : mainLoop)
    {
        qDebug() << el;
    }
    qDebug() << "\n";
    return mainLoop;
}
