#include "Edge.h"

int Edge::s_margin = 3;
LineType Edge::s_type = LineType::Library;

Edge::Edge(QSharedPointer<Vertex> v1, QSharedPointer<Vertex> v2, int thic) : first(v1), second(v2), thickness(thic) {}

void Edge::paint(QSharedPointer<QPainter> painter) const
{
    drawLine(painter, QPoint(first->x, first->y), QPoint(second->x, second->y), s_type);
}

bool operator==(const Edge& e1, const Edge& e2)
{
    if (*e1.first == *e2.first)
    {
        return *e1.second == *e2.second;
    }
    else if (*e1.first == *e2.second)
    {
        return *e1.second == *e2.first;
    }
    return false;
}

