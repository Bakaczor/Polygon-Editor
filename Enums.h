#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

class Algorithm : public QObject {
    Q_OBJECT
    public:
        enum class Enum { Library, Bresenham };
        Q_ENUM(Enum)
};

class Orientation : public QObject {
    Q_OBJECT
    public:
        enum class Enum { Horizontal, Vertical, None };
        Q_ENUM(Enum)
};

enum class Geometry
{
    Polygon, Edge, Vertex, None
};

enum class Clock
{
    Clockwise, Anticlockwise, Collinear
};

#endif
