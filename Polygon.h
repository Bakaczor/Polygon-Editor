#include <QPainter>

#include "Edge.h"

class Polygon
{
    public:
        QList<QSharedPointer<Vertex>> vertices;
        QList<Edge> edges;
        bool isSelected;

        Polygon(QList<Vertex> verts);

        void drag(int dx, int dy);
        void paint(QSharedPointer<QPainter> painter) const;
        void select();
        void unselect();
        //void addVertex(Vertex* v);
        bool removeVertex(Vertex* v);
        bool contains(const QPoint& p) const;
        Vertex* checkVertices(const QPoint& p);
        Edge* checkEdges(const QPoint& p);
};
