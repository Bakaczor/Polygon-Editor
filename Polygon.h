#include <QPainter>

#include "Edge.h"

class Polygon
{
    public:
        QList<QSharedPointer<Vertex>> vertices;
        QList<Edge> edges;

        Polygon(QList<Vertex> verts);

        void drag(int dx, int dy);
        void paint(QSharedPointer<QPainter> painter, const Algorithm::Enum& type) const;
        void select();
        void unselect();
        void insertVertex(int x, int y, int eIdx);
        bool removeVertex(int vIdx);
        bool contains(const QPoint& p) const;
        int checkVertices(const QPoint& p) const;
        int checkEdges(const QPoint& p) const;
        void dragVertex(int x, int y, int currVerIdx);
        void dragEdge(int dx, int dy, int currEdgIdx);
        void updateOffset(int offset);

    private:
        uint m_offset;
        QPolygon m_offsetPoly;
};
