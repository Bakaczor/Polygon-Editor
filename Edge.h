#include <QPainter>

#include "Vertex.h"
#include "Functions.h"

class Edge {
    public:
        const static uint s_margin;
        Vertex* first;
        Vertex* second;
        QColor color;
        int thicc;

        Edge(Vertex* v1, Vertex* v2);

        void drag(int dx, int dy);
        void paint(QSharedPointer<QPainter> painter, const Algorithm::Enum& type) const;
        void select();
        void unselect();

        Orientation::Enum getOrientation() const;
        void setOrientation(Orientation::Enum newOrientation);

        bool contains(const QPoint& p) const;
        friend bool operator==(const Edge& e1, const Edge& e2);

    private:
        Orientation::Enum m_orient;

        QPoint getMiddle() const;
};

