#include <QtMath>

#include "Edge.h"

int Edge::s_margin = 5;
Algorithm Edge::s_type = Algorithm::Library;


Edge::Edge(Vertex* v1, Vertex* v2, int thic) : m_first(v1), m_second(v2), m_thicc(thic), m_orient(Orientation::None)
{
    m_A = v1->Y - v2->Y;
    m_B = v2->X - v1->X;
    m_C = v1->X * v2->Y - v2->X * v1->Y;
}

void Edge::drag(int dx, int dy)
{
    m_first->X += dx;
    m_first->Y += dy;
    m_second->X += dx;
    m_second->Y += dy;
}

void Edge::paint(QSharedPointer<QPainter> painter) const
{
    drawLine(painter, QPoint(m_first->X, m_first->Y), QPoint(m_second->X, m_second->Y), s_type);
}

bool operator==(const Edge& e1, const Edge& e2)
{
    if (e1.m_first == e2.m_first)
    {
        return e1.m_second == e2.m_second;
    }
    else if (e1.m_first == e2.m_second)
    {
        return e1.m_second == e2.m_first;
    }
    return false;
}

bool Edge::contains(const QPoint& p) const
{
    int A = p.x() - m_first->X;
    int B = p.y() - m_first->Y;
    int C = m_second->X - m_first->X;
    int D = m_second->Y - m_first->Y;
    long dot = A * C + B * D;
    long lensq = C * C  + D * D;

    int xx, yy;
    if (dot <= 0)
    {
        xx = m_first->X;
        yy = m_first->Y;
    }
    else if (dot >= lensq)
    {
        xx = m_second->X;
        yy = m_second->Y;
    }
    else
    {
        xx = m_first->X + C * dot / lensq;
        yy = m_first->Y + D * dot / lensq;
    }
    int dx = p.x() - xx;
    int dy = p.y() - yy;

    return dx * dx + dy * dy < s_margin * s_margin;
}
