#include <QPainter>
#include <QtMath>

class Vertex
{
    public:
        static int s_margin;
        int X;
        int Y;

        Vertex(int x, int y, int thic = 5);

        void drag(int x, int y);
        void paint(QSharedPointer<QPainter> painter) const;

        operator QPoint() const;
        friend bool operator==(const Vertex& v1, const Vertex& v2);

    private:
        int m_thicc;
};
