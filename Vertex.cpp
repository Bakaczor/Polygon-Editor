#include "Vertex.h"

int Vertex::s_margin = 7;

Vertex::Vertex(int x, int y, int thic): x(x), y(y), thickness(thic) {}

void Vertex::drag(int nx, int ny)
{
    x = nx;
    y = ny;
}

void Vertex::paint(QSharedPointer<QPainter> painter) const
{
    QPen p;
    p.setWidth(thickness);
    painter->setPen(p);
    painter->drawPoint(QPoint(x, y));
}

bool operator==(const Vertex& v1, const Vertex& v2)
{
    return qFabs(v1.x - v2.x) < v1.s_margin && qFabs(v1.y - v2.y) < v2.s_margin;
}

