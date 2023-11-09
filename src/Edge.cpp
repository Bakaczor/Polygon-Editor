#include "Edge.h"
#include "Icon.h"
#include "Functions.h"

Edge::Edge(Vertex* v1, Vertex* v2, Orientation::Enum orient) :
    first(v1), second(v2),
    m_orient(orient), m_softOrient(orient),
    thicc(2), color(QColor(0, 0, 0, 255)) {}

void Edge::drag(int dx, int dy)
{
    first->X += dx;
    first->Y += dy;
    second->X += dx;
    second->Y += dy;
}

void Edge::paint(QSharedPointer<QPainter> painter, const Algorithm::Enum& type) const
{
    drawLine(painter, static_cast<QPoint>(*first), static_cast<QPoint>(*second), type, thicc, color);
    if (m_orient != Orientation::Enum::None)
    {
        Icon i;
        i.setPosition(getMiddle());
        i.setSource(m_orient);
        i.paint(painter.get());
    }
    // new functionality
    else if (m_softOrient != Orientation::Enum::None)
    {
        Icon i;
        i.setPosition(getMiddle());
        i.setSource(m_softOrient);
        i.paint(painter.get());
    }
}

void Edge::select()
{
    thicc = 4;
    color = QColor(255, 0, 0, 255);
}

void Edge::unselect()
{
    thicc = 2;
    color = QColor(0, 0, 0, 255);
}

Orientation::Enum Edge::getOrientation() const
{
    return m_orient;
}

void Edge::setOrientation(Orientation::Enum newOrientation)
{
    if (m_orient != newOrientation)
    {
        m_orient = newOrientation;
        if (newOrientation == Orientation::Enum::Horizontal)
        {
            const int dy = (second->Y - first->Y) >> 1;
            first->Y += dy;
            second->Y -= dy;
        }
        else if (newOrientation == Orientation::Enum::Vertical)
        {
            const int dx = (second->X - first->X) >> 1;
            first->X += dx;
            second->X -= dx;
        }
    }
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

// https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
bool Edge::contains(const QPoint& p) const
{
    int A = p.x() - first->X;
    int B = p.y() - first->Y;
    int C = second->X - first->X;
    int D = second->Y - first->Y;
    long dot = A * C + B * D;
    long lensq = C * C + D * D;

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

Orientation::Enum Edge::getSoftOrientation() const
{
    return m_softOrient;
}

void Edge::setSoftOrientation(Orientation::Enum newOrientation)
{
    m_softOrient = newOrientation;
}

QPoint Edge::getMiddle() const
{
    const int dx = (second->X - first->X) >> 1;
    const int dy = (second->Y - first->Y) >> 1;
    return QPoint(first->X + dx, first->Y + dy);
}
