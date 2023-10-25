#include "Polygon.h"

Polygon::Polygon(QList<Vertex> verts) : isSelected(false)
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
    edges.append(Edge(vertices.first().get(), vertices.last().get()));
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
    for (int i = 0; i < vertices.count(); i++)
    {
        vertices.at(i)->paint(painter);
        edges.at(i).paint(painter);
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

bool Polygon::removeVertex(Vertex* v)
{
    if (vertices.count() <= 3) { return false; }
    int i = vertices.indexOf(QSharedPointer<Vertex>(new Vertex(v->X, v->Y)));
    int j = i == 0 ? edges.count() - 1 : i - 1;
    int k = i == vertices.count() - 1 ? 0 : i + 1;
    edges[j].second = vertices.at(k).data();
    //vertices.removeAt(i);
    //edges.removeAt(i);
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

Vertex* Polygon::checkVertices(const QPoint& p)
{
    // paralellize
    for (auto& pv : vertices)
    {
        if (Vertex(p.x(), p.y()) == *pv)
        {
            return pv.get();
        }
    }
    return nullptr;
}

Edge* Polygon::checkEdges(const QPoint& p)
{
    // paralellize
    for (Edge& e : edges)
    {
        if (e.contains(Vertex(p.x(), p.y())))
        {
            return &e;
        }
    }
    return nullptr;
}
