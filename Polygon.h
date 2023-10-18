#include <QPolygon>
#include <QList>
#include <QPainter>

#include "Edge.h"

class Polygon
{
    public:
        static int s_margin;
        QList<Vertex> vertices;
        QList<Edge> edges;

        Polygon(QList<Vertex> vertices);
        void paint(QPainter* painter);
};

