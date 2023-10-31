#include <QPainter>

#include "Edge.h"
#include "OffsetPolygon.h"

class Polygon
{
    public:
        QVector<QSharedPointer<Vertex>> vertices;
        QVector<Edge> edges;

        Polygon(QVector<Vertex> verts);

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
        OffsetPolygon m_offsetPoly;
};
