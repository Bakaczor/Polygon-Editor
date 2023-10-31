#include <QPainter>
#include <QSharedPointer>

class Vertex
{
    public:
        const static uint s_margin = 7;
        QColor color;
        int thicc;
        int X;
        int Y;

        Vertex(int x, int y);

        void drag(int x, int y);
        void paint(QSharedPointer<QPainter> painter) const;
        void select();
        void unselect();

        operator QPoint() const;
        friend bool operator==(const Vertex& v1, const Vertex& v2);
};
