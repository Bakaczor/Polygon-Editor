#include "Polygon.h"

int Polygon::s_margin = 3;

Polygon::Polygon(QList<Vertex> verts) : vertices(verts)
{
    for (int i = 0; i < vertices.count() - 1; i++)
    {
        auto v1 = QSharedPointer<Vertex>(&(vertices[i]));
        auto v2 = QSharedPointer<Vertex>(&(vertices[i + 1]));
        edges.append(Edge(v1, v2));
    }
    edges.append(Edge(QSharedPointer<Vertex>(&(vertices.first())), QSharedPointer<Vertex>(&(vertices.last()))));
}

void Polygon::paint(QSharedPointer<QPainter> painter) const
{
    for (int i = 0; i < vertices.count(); i++)
    {
        vertices.at(i).paint(painter);
        edges.at(i).paint(painter);
    }
}
