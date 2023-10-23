#include "Polygon.h"

Polygon::Polygon(QList<Vertex> verts) : vertices(verts)
{
    // create edges
    for (int i = 0; i < vertices.count() - 1; i++)
    {
        edges.append(Edge(&(vertices[i]), &(vertices[i + 1])));
    }
    edges.append(Edge(&vertices.first(), &vertices.last()));
}

void Polygon::drag(int dx, int dy)
{
    // paralellize
    for (Vertex& v : vertices)
    {
        v.X += dx;
        v.Y += dy;
    }
}

void Polygon::paint(QSharedPointer<QPainter> painter) const
{
    // paralellize
    for (int i = 0; i < vertices.count(); i++)
    {
        vertices.at(i).paint(painter);
        edges.at(i).paint(painter);
    }
}

bool Polygon::contains(const QPoint& p) const
{
    QVector<QPoint> points;
    points.reserve(vertices.count());
    for (const Vertex& v : vertices)
    {
        points.append(static_cast<QPoint>(v));
    }
    return QPolygon(points).containsPoint(p, Qt::OddEvenFill);
}

Vertex* Polygon::checkVertices(const QPoint& p)
{
    // paralellize
    for (Vertex& v : vertices)
    {
        if (Vertex(p.x(), p.y()) == v)
        {
            return &v;
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
