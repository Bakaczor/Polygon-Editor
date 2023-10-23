#include "Vertex.h"

int Vertex::s_margin = 10;

Vertex::Vertex(int x, int y, int thicc): X(x), Y(y), m_thicc(thicc) {}

void Vertex::drag(int x, int y)
{
    X = x;
    Y = y;
}

void Vertex::paint(QSharedPointer<QPainter> painter) const
{
    QPen p;
    p.setWidth(m_thicc);
    painter->setPen(p);
    painter->drawPoint(QPoint(X, Y));
}

Vertex::operator QPoint() const
{
    return QPoint(X, Y);
}

bool operator==(const Vertex& v1, const Vertex& v2)
{
    return qFabs(v1.X - v2.X) < v1.s_margin && qFabs(v1.Y - v2.Y) < v2.s_margin;
}

