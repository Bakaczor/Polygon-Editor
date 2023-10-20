#include <QPainter>
#include <QSharedPointer>

#include "Vertex.h"
#include "Functions.h"

class Edge {
    public:
        static int s_margin;
        static LineType s_type;
        QSharedPointer<Vertex> first;
        QSharedPointer<Vertex> second;
        int thickness;

        Edge(QSharedPointer<Vertex> v1, QSharedPointer<Vertex> v2, int thic = 2);

        //void drag(int x, int y);
        void paint(QSharedPointer<QPainter> painter) const;

        friend bool operator==(const Edge& e1, const Edge& e2);

};

