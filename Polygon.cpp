#include "Polygon.h"

int Polygon::s_margin = 3;

Polygon::Polygon(QList<Vertex> verts) : vertices(verts)
{
    // create edges
    for (int i = 0; i < vertices.count() - 1; i++)
    {
        edges.append(Edge(&(vertices[i]), &(vertices[i + 1])));
    }
    edges.append(Edge(&vertices.first(), &vertices.last()));

    rebuild();
}

void Polygon::paint(QSharedPointer<QPainter> painter) const
{
    for (int i = 0; i < vertices.count(); i++)
    {
        vertices.at(i).paint(painter);
        edges.at(i).paint(painter);
    }
}

bool Polygon::contains(const QPoint& p) const
{
    return m_polygon.containsPoint(p, Qt::OddEvenFill);
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

void Polygon::rebuild()
{
    QVector<QPoint> points;
    points.reserve(vertices.count());
    for (const Vertex& v : vertices)
    {
        points.append(static_cast<QPoint>(v));
    }
    m_polygon = QPolygon(points);
}
