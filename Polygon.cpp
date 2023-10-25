#include "Polygon.h"

Polygon::Polygon(QList<Vertex> verts)
{
    for (const Vertex v : verts)
    {
        vertices.append(QSharedPointer<Vertex>(new Vertex(v)));
    }
    // create edges
    for (int i = 0; i < vertices.count() - 1; i++)
    {
        edges.append(Edge(vertices.at(i).get(), vertices.at(i + 1).get()));
    }
    edges.append(Edge(vertices.last().get(), vertices.first().get()));
}

void Polygon::drag(int dx, int dy)
{
    // paralellize
    for (auto& v : vertices)
    {
        v->X += dx;
        v->Y += dy;
    }
}

void Polygon::paint(QSharedPointer<QPainter> painter) const
{
    // paralellize
    for (const Edge& e : edges)
    {
        e.paint(painter);
    }
    for (const auto& pv : vertices)
    {
        pv->paint(painter);
    }
}

// paralellize
void Polygon::select()
{
    for (int i = 0; i < vertices.count(); i++)
    {
        vertices[i]->select();
        edges[i].select();
    }
}

void Polygon::unselect()
{
    for (int i = 0; i < vertices.count(); i++)
    {
        vertices[i]->unselect();
        edges[i].unselect();
    }
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
}

bool Polygon::removeVertex(int vIdx)
{
    if (vertices.count() <= 3) { return false; }

    int peIdx = vIdx == 0 ? edges.count() - 1 : vIdx - 1;
    int nvIdx = vIdx == vertices.count() - 1 ? 0 : vIdx + 1;
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

int Polygon::checkVertices(const QPoint& p)
{
    // paralellize
    for (int i = 0; i < vertices.count(); i++)
    {
        if (*vertices.at(i) == Vertex(p.x(), p.y())) { return i; }
    }
    return -1;
}

int Polygon::checkEdges(const QPoint& p)
{
    // paralellize
    for (int i = 0; i < edges.count(); i++)
    {
        if (edges.at(i).contains(Vertex(p.x(), p.y()))) { return i; }
    }
    return -1;
}
