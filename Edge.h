#include <QPainter>

#include "Vertex.h"

class Edge {
    public:
        static int s_margin;
        Vertex* first;
        Vertex* second;
        int thickness;

        Edge(Vertex* first, Vertex* second);
        Edge(const Edge& other);
        Edge(const Edge&& other) noexcept;
        Edge& operator=(const Edge& other);
        Edge& operator=(const Edge&& other) noexcept;

        void drag(int x, int y);
        void paint(QPainter* painter);

        friend bool operator==(const Edge& e1, const Edge& e2);

};

