#include "Vertex.h"

const int Vertex::s_margin = 7;

Vertex::Vertex(int x, int y): X(x), Y(y), thicc(6), color(QColor(0, 0, 255, 255)) {}

void Vertex::drag(int x, int y)
{
    X = x;
    Y = y;
}

void Vertex::paint(QSharedPointer<QPainter> painter) const
{
    QPen p;
    p.setWidth(thicc);
    p.setColor(color);
    painter->setPen(p);
    painter->drawPoint(QPoint(X, Y));
}

void Vertex::select()
{
    thicc = 8;
    color = QColor(255, 0, 0, 255);
}

void Vertex::unselect()
{
    thicc = 6;
    color = QColor(0, 0, 255, 255);
}

Vertex::operator QPoint() const
{
    return QPoint(X, Y);
}

bool operator==(const Vertex& v1, const Vertex& v2)
{
    return qFabs(v1.X - v2.X) < v1.s_margin && qFabs(v1.Y - v2.Y) < v2.s_margin;
}

