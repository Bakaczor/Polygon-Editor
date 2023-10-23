#include <QPainter>
#include <QSharedPointer>

#include "Vertex.h"
#include "Functions.h"

class Edge {
    public:
        static int s_margin;
        static Algorithm s_type;

        Edge(Vertex* v1, Vertex* v2, int thic = 2);

        void drag(int dx, int dy);
        void paint(QSharedPointer<QPainter> painter) const;
        bool contains(const QPoint& p) const;
        friend bool operator==(const Edge& e1, const Edge& e2);

    private:
        int m_A;
        int m_B;
        long m_C;
        int m_thicc;
        Vertex* m_first;
        Vertex* m_second;
        Orientation m_orient;
};

