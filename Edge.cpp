#include <QtMath>

#include "Edge.h"
#include "qdebug.h"

int Edge::s_margin = 7;
Algorithm Edge::s_type = Algorithm::Library;

// NOTE: Y coordinates must be sometimes negated to have mathematical consistency

Edge::Edge(Vertex* v1, Vertex* v2, int thic) : m_first(v1), m_second(v2), m_thicc(thic)
{
    m_A = v1->Y - v2->Y;
    m_B = v2->X - v1->X;
    m_C = v1->X * v2->Y - v2->X * v1->Y;
    m_length = sqrt(m_A * m_A + m_B * m_B);
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

// FIX IT - the distance is too big
bool Edge::contains(const QPoint& p) const
{
    int minX = m_first->X < m_second->X ? m_first->X : m_second->X;
    int maxX = minX == m_second->X ? m_first->X : m_second->X;
    if (p.x() < minX && qFabs(p.x() - minX) > s_margin) { return false; }
    else if (p.x() > maxX && qFabs(p.x() - maxX) > s_margin) { return false; }

    int minY = m_first->Y < m_second->Y ? m_first->Y : m_second->Y;
    int maxY = minY == m_second->Y ? m_first->Y : m_second->Y;
    if (p.y() < minY && qFabs(p.y() - minY) > s_margin) { return false; }
    else if (p.y() > maxY && qFabs(p.y() - maxY) > s_margin) { return false; }

    qDebug() << "Distance: " << qFabs(m_A * p.x() - m_B * p.y() + m_C) / m_length << "\n";
    return qFabs(m_A * p.x() - m_B * p.y() + m_C) / m_length < s_margin;
}
