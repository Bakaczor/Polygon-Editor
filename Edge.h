#include <QPainter>

#include "Vertex.h"
#include "Functions.h"

class Edge {
    public:
        const static int s_margin;
        static Algorithm s_type;
        Vertex* first;
        Vertex* second;
        Orientation orient;
        QColor color;
        int thicc;

        Edge(Vertex* v1, Vertex* v2);

        void drag(int dx, int dy);
        void paint(QSharedPointer<QPainter> painter) const;
        void select();
        void unselect();
        bool contains(const QPoint& p) const;
        friend bool operator==(const Edge& e1, const Edge& e2);

    private:
        int m_A;
        int m_B;
        long m_C;
};

