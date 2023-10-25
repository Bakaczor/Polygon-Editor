#include <QtMath>

#include "Edge.h"

const int Edge::s_margin = 5;
Algorithm Edge::s_type = Algorithm::Library;


Edge::Edge(Vertex* v1, Vertex* v2) : first(v1), second(v2), orient(Orientation::None),
    thicc(3), color(QColor(0, 0, 0, 255))
{
    m_A = v1->Y - v2->Y;
    m_B = v2->X - v1->X;
    m_C = v1->X * v2->Y - v2->X * v1->Y;
}

void Edge::drag(int dx, int dy)
{
    first->X += dx;
    first->Y += dy;
    second->X += dx;
    second->Y += dy;
}

void Edge::paint(QSharedPointer<QPainter> painter) const
{
    drawLine(painter, (QPoint)*first, (QPoint)*second, s_type, thicc, color);
}

void Edge::select()
{
    thicc = 4;
    color = QColor(255, 0, 0, 255);
}

void Edge::unselect()
{
    thicc = 3;
    color = QColor(0, 0, 0, 255);
}

bool operator==(const Edge& e1, const Edge& e2)
{
    if (e1.first == e2.first)
    {
        return e1.second == e2.second;
    }
    else if (e1.first == e2.second)
    {
        return e1.second == e2.first;
    }
    return false;
}

bool Edge::contains(const QPoint& p) const
{
    int A = p.x() - first->X;
    int B = p.y() - first->Y;
    int C = second->X - first->X;
    int D = second->Y - first->Y;
    long dot = A * C + B * D;
    long lensq = C * C  + D * D;

    int xx, yy;
    if (dot <= 0)
    {
        xx = first->X;
        yy = first->Y;
    }
    else if (dot >= lensq)
    {
        xx = second->X;
        yy = second->Y;
    }
    else
    {
        xx = first->X + C * dot / lensq;
        yy = first->Y + D * dot / lensq;
    }
    int dx = p.x() - xx;
    int dy = p.y() - yy;

    return dx * dx + dy * dy < s_margin * s_margin;
}
