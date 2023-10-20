#include <QPainter>

#include "Edge.h"

class Polygon
{
    public:
        static int s_margin;
        //QPolygon poly;
        QList<Vertex> vertices;
        QList<Edge> edges;

        Polygon(QList<Vertex> verts);
        void paint(QSharedPointer<QPainter> painter) const;
};

