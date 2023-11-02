#pragma once

#include <QPainter>
#include <QSharedPointer>

/*!
 * \brief The Vertex class
 * This class represents a vertex in a polygon, with properties such as position, color, and thickness.
 */
class Vertex
{
public:
    const static uint s_margin = 7;
    QColor color;
    int thicc;
    int X;
    int Y;

    /*!
     * \brief Constructs a Vertex object
     * \param x The x-coordinate of the vertex
     * \param y The y-coordinate of the vertex
     */
    Vertex(int x, int y);

    /*!
     * \brief Moves the vertex to a specified position
     * \param x The new x-coordinate of the vertex
     * \param y The new y-coordinate of the vertex
     */
    void drag(int x, int y);

    /*!
     * \brief Paints the vertex on a canvas
     * \param painter A shared pointer to a QPainter object
     */
    void paint(QSharedPointer<QPainter> painter) const;

    /*!
     * \brief Selects the vertex
     * This method changes the color and thickness of the vertex to indicate that it's selected.
     */
    void select();

    /*!
     * \brief Unselects the vertex
     * This method changes the color and thickness of the vertex to indicate that it's not selected.
     */
    void unselect();

    /*!
     * \brief Converts the vertex to a QPoint object
     * \return A QPoint object representing the position of the vertex
     */
    operator QPoint() const;

    /*!
     * \brief Checks if two vertices are equal
     * \param v1 The first vertex to compare
     * \param v2 The second vertex to compare
     * \return True if the vertices are equal, false otherwise
     */
    friend bool operator==(const Vertex& v1, const Vertex& v2);
};
