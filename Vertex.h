#include <QPainter>
#include <QtMath>

class Vertex
{
    public:
        static int s_margin;
        int x;
        int y;
        int thickness;

        Vertex(int x, int y, int thic = 5);

        void drag(int nx, int ny);
        void paint(QSharedPointer<QPainter> painter) const;

        friend bool operator==(const Vertex& v1, const Vertex& v2);
};
