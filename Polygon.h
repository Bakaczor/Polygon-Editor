#include <QPainter>

#include "Edge.h"

class Polygon
{
    public:
        QList<QSharedPointer<Vertex>> vertices;
        QList<Edge> edges;

        Polygon(QList<Vertex> verts);

        void drag(int dx, int dy);
        void paint(QSharedPointer<QPainter> painter) const;
        void select();
        void unselect();
        void insertVertex(int x, int y, int eIdx);
        bool removeVertex(int vIdx);
        bool contains(const QPoint& p) const;
        int checkVertices(const QPoint& p);
        int checkEdges(const QPoint& p);
};
