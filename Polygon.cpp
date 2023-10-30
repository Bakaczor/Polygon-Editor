#include <QtConcurrent/QtConcurrent>

//#include "earcut.hpp"

#include "Polygon.h"
#include "Geometry.h"

Polygon::Polygon(QList<Vertex> verts)
{
    // add vertices
    for (const Vertex v : verts)
    {
        vertices.append(QSharedPointer<Vertex>(new Vertex(v)));
    }

    // create edges
    for (uint i = 0; i < vertices.count() - 1; i++)
    {
        edges.append(Edge(vertices.at(i).get(), vertices.at(i + 1).get()));
    }
    edges.append(Edge(vertices.last().get(), vertices.first().get()));
}

void Polygon::drag(int dx, int dy)
{
    QtConcurrent::blockingMap(vertices, [&dx, &dy](auto& pv) {
        pv->X += dx;
        pv->Y += dy;
    });
    updateOffset(m_offset);
}

void Polygon::paint(QSharedPointer<QPainter> painter, const Algorithm::Enum& type) const
{
    if (!m_offsetPoly.empty())
    {
        QPen p;
        p.setWidth(2);
        p.setColor(QColor(0, 0, 0, 255));
        painter->setPen(p);
        painter->drawPolygon(m_offsetPoly);
    }
    for (const Edge& e : edges)
    {
        e.paint(painter, type);
    }
    for (const auto& pv : vertices)
    {
        pv->paint(painter);
    }
}

void Polygon::select()
{
    QtConcurrent::map(vertices, [](auto& pv) {
        pv->select();
    });

    QtConcurrent::blockingMap(edges, [](Edge& e) {
        e.select();
    });
}

void Polygon::unselect()
{
    QtConcurrent::map(vertices, [](auto& pv) {
        pv->unselect();
    });

    QtConcurrent::blockingMap(edges, [](Edge& e) {
        e.unselect();
    });
}

void Polygon::insertVertex(int x, int y, int eIdx)
{
    auto pv = QSharedPointer<Vertex>(new Vertex(x, y));
    int pvIdx = eIdx;
    int nvIdx = eIdx == vertices.count() - 1 ? 0 : eIdx + 1;

    Edge pe(vertices.at(pvIdx).get(), pv.get());
    Edge ne(pv.get(), vertices.at(nvIdx).get());

    if (nvIdx)
    {
        vertices.insert(nvIdx, pv);
    }
    else
    {
        vertices.append(pv);
    }
    edges.removeAt(eIdx);
    edges.insert(eIdx, pe);
    edges.insert(eIdx + 1, ne);

    updateOffset(m_offset);
}

bool Polygon::removeVertex(int vIdx)
{
    if (vertices.count() <= 3) { return false; }

    int peIdx = vIdx == 0 ? edges.count() - 1 : vIdx - 1;
    int nvIdx = vIdx == vertices.count() - 1 ? 0 : vIdx + 1;
    edges[peIdx].setOrientation(Orientation::Enum::None);
    if (*vertices.at(vIdx) == *edges.at(peIdx).first)
    {
        edges[peIdx].first = vertices.at(nvIdx).data();
    }
    else
    {
        edges[peIdx].second = vertices.at(nvIdx).data();
    }
    vertices.removeAt(vIdx);
    edges.removeAt(vIdx);

    updateOffset(m_offset);
    return true;
}

bool Polygon::contains(const QPoint& p) const
{
    QVector<QPoint> points;
    points.reserve(vertices.count());
    for (const auto& pv : vertices)
    {
        points.append(static_cast<QPoint>(*pv));
    }
    return QPolygon(points).containsPoint(p, Qt::OddEvenFill);
}

int Polygon::checkVertices(const QPoint& p) const
{
    for (uint i = 0; i < vertices.count(); i++)
    {
        if (*vertices.at(i) == Vertex(p.x(), p.y())) { return i; }
    }
    return -1;
}

int Polygon::checkEdges(const QPoint& p) const
{
    QVector<bool> result(edges.count(), false);
    QtConcurrent::blockingMap(edges, [this, &result, &p](const Edge& e) {
        std::ptrdiff_t i = std::distance(&edges.at(0), &e);
        result[i] = e.contains(p);
    });

    for (int i = 0; i < result.count(); i++)
    {
        if (result.at(i)) { return i; }
    }
    return -1;
}

void Polygon::dragVertex(int x, int y, int currVerIdx)
{
    int peIdx = currVerIdx == 0 ? edges.count() - 1 : currVerIdx - 1;
    int pvIdx = currVerIdx == 0 ? vertices.count() - 1 : currVerIdx - 1;

    if (edges.at(peIdx).getOrientation() == Orientation::Enum::Horizontal)
    {
        vertices[pvIdx]->drag(vertices.at(pvIdx)->X, y);
    }
    else if (edges.at(peIdx).getOrientation() == Orientation::Enum::Vertical)
    {
        vertices[pvIdx]->drag(x, vertices.at(pvIdx)->Y);
    }

    int neIdx = currVerIdx;
    int nvIdx = currVerIdx == vertices.count() - 1 ? 0 : currVerIdx + 1;

    if (edges.at(neIdx).getOrientation() == Orientation::Enum::Horizontal)
    {
        vertices[nvIdx]->drag(vertices.at(nvIdx)->X, y);
    }
    else if (edges.at(neIdx).getOrientation() == Orientation::Enum::Vertical)
    {
        vertices[nvIdx]->drag(x, vertices.at(nvIdx)->Y);
    }

    vertices[currVerIdx]->drag(x, y);
    updateOffset(m_offset);
}

void Polygon::dragEdge(int dx, int dy, int currEdgIdx)
{
    int peIdx = currEdgIdx == 0 ? edges.count() - 1 : currEdgIdx - 1;
    int neIdx = currEdgIdx == edges.count() - 1 ? 0 : currEdgIdx + 1;

    // right triangle case
    if (edges.count() == 3 && edges.at(peIdx).getOrientation() != Orientation::Enum::None && edges.at(neIdx).getOrientation() != Orientation::Enum::None)
    {
        this->drag(dx, dy);
        return;
    }
    edges[currEdgIdx].drag(dx, dy);

    int pvIdx = currEdgIdx == 0 ? vertices.count() - 1 : currEdgIdx - 1;
    int cpvIdx = currEdgIdx;

    if (edges.at(peIdx).getOrientation() == Orientation::Enum::Horizontal)
    {
        vertices[pvIdx]->drag(vertices.at(pvIdx)->X, vertices.at(cpvIdx)->Y);
    }
    else if (edges.at(peIdx).getOrientation() == Orientation::Enum::Vertical)
    {
        vertices[pvIdx]->drag(vertices.at(cpvIdx)->X, vertices.at(pvIdx)->Y);
    }

    int nvIdx = (currEdgIdx + 2) % vertices.count();
    int cnvIdx = currEdgIdx == vertices.count() - 1 ? 0 : currEdgIdx + 1;

    if (edges.at(neIdx).getOrientation() == Orientation::Enum::Horizontal)
    {
        vertices[nvIdx]->drag(vertices.at(nvIdx)->X, vertices.at(cnvIdx)->Y);
    }
    else if (edges.at(neIdx).getOrientation() == Orientation::Enum::Vertical)
    {
        vertices[nvIdx]->drag(vertices.at(cnvIdx)->X, vertices.at(nvIdx)->Y);
    }
    updateOffset(m_offset);
}

void Polygon::updateOffset(int offset)
{
    m_offset = offset;
    m_offsetPoly.clear();
    if (offset == 0) { return; }

    QVector<QPoint> points;
    points.reserve(vertices.count());
    for (const auto& pv : vertices)
    {
        points.append(static_cast<QPoint>(*pv));
    }

    if (!polygonSign(points))
    {
        m_offset = -m_offset;
    }

    uint n = points.count();
    QVector<QPair<QPoint, QPoint>> segments;
    segments.reserve(vertices.count());
    for (uint i = 0; i < n - 1; i++)
    {
        segments.append(offsetSegment(points.at(i), points.at(i + 1), m_offset));
    }
    segments.append(offsetSegment(points.constLast(), points.constFirst(), m_offset));

    for (uint i = 0; i < n - 1; i++)
    {
        std::optional<QPoint> opt = lineIntersection(segments.at(i).first, segments.at(i).second, segments.at(i + 1).first, segments.at(i + 1).second);
        if (opt.has_value())
        {
            m_offsetPoly.append(opt.value());
        }
    }
    std::optional<QPoint> opt = lineIntersection(segments.constLast().first, segments.constLast().second, segments.constFirst().first, segments.constFirst().second);
    if (opt.has_value())
    {
        m_offsetPoly.append(opt.value());
    }
}

/*
    using Point = std::array<int, 2>;
    std::vector<Point> polyline;
    polyline.reserve(vertices.count());
    for (const auto& pv : vertices)
    {
        polyline.push_back({pv->X, pv->Y});
    }
    std::vector<std::vector<Point>> polygon;
    polygon.push_back(polyline);
    std::vector<int> indices = mapbox::earcut<int>(polygon);
*/
