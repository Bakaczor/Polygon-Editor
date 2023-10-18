#include <QtMath>
#include "Vertex.h"

int Vertex::s_margin = 3;

bool operator==(const Vertex& v1, const Vertex& v2)
{
    return qFabs(v1.x - v2.x) < v1.s_margin && qFabs(v1.y - v2.y) < v2.s_margin;
}

