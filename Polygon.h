#include <QPainter>

#include "Edge.h"

class Polygon
{
    public:
        QList<Vertex> vertices;
        QList<Edge> edges;

        Polygon(QList<Vertex> verts);

        void drag(int dx, int dy);
        void paint(QSharedPointer<QPainter> painter) const;
        bool contains(const QPoint& p) const;
        Vertex* checkVertices(const QPoint& p);
        Edge* checkEdges(const QPoint& p);
};
