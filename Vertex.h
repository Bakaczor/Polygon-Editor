#include <QPainter>
#include <QtMath>

class Vertex
{
    public:
        static int s_margin;
        int x;
        int y;
        int thickness;

        Vertex(int x, int y);
        Vertex(const Vertex& other);
        Vertex(const Vertex&& other) noexcept;
        Vertex& operator=(const Vertex& other);
        Vertex& operator=(const Vertex&& other) noexcept;

        void drag(int x, int y);
        void paint(QPainter* painter);

        friend bool operator==(const Vertex& v1, const Vertex& v2);
};
