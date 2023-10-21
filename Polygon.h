#include <QPainter>

#include "Edge.h"

class Polygon
{
    public:
        static int s_margin;
        QList<Vertex> vertices;
        QList<Edge> edges;

        Polygon(QList<Vertex> verts);
        void paint(QSharedPointer<QPainter> painter) const;

        bool contains(const QPoint& p) const;
        Vertex* checkVertices(const QPoint& p);
        Edge* checkEdges(const QPoint& p);

    private:
        QPolygon m_polygon;
};
