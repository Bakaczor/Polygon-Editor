#pragma once

#include <QObject>

/*!
 * \brief The Algorithm class
 * This class represents an enumeration of algorithms.
 */
class Algorithm : public QObject {
    Q_OBJECT
public:
    /*!
     * \brief The Algorithm enumeration
     * This enumeration represents different algorithms.
     */
    enum class Enum { Library, Bresenham, SymmetricDoubleStep, GuptaSproull };
    Q_ENUM(Enum)
};

/*!
 * \brief The Orientation class
 * This class represents an enumeration of orientations.
 */
class Orientation : public QObject {
    Q_OBJECT
public:
    /*!
     * \brief The Orientation enumeration
     * This enumeration represents different orientations.
     */
    enum class Enum { Horizontal, Vertical, None };
    Q_ENUM(Enum)
};

/*!
 * \brief The Geometry enumeration
 * This enumeration represents different geometric shapes.
 */
enum class Geometry
{
    Polygon, Edge, Vertex, None
};
