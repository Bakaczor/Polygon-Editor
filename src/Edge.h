#pragma once

#include "Vertex.h"
#include "Enums.h"

/*!
 * \brief The Edge class
 * This class represents an edge in a polygon, with two vertices and additional properties such as color and thickness.
 */
class Edge {
public:
    const static uint s_margin = 5;
    Vertex* first;
    Vertex* second;
    QColor color;
    int thicc;

    /*!
     * \brief Constructs an Edge object
     * \param v1 The first vertex of the edge
     * \param v2 The second vertex of the edge
     */
    Edge(Vertex* v1, Vertex* v2, Orientation::Enum orient = Orientation::Enum::None);

    /*!
     * \brief Moves the edge by a specified amount
     * \param dx The amount to move in the x direction
     * \param dy The amount to move in the y direction
     */
    void drag(int dx, int dy);

    /*!
     * \brief Paints the edge on a canvas
     * \param painter A shared pointer to a QPainter object
     * \param type The type of algorithm to use for painting
     */
    void paint(QSharedPointer<QPainter> painter, const Algorithm::Enum& type) const;

    /*!
     * \brief Selects the edge
     * This method changes the color and thickness of the edge to indicate that it's selected.
     */
    void select();

    /*!
     * \brief Unselects the edge
     * This method changes the color and thickness of the edge to indicate that it's not selected.
     */
    void unselect();

    /*!
     * \brief Gets the orientation of the edge
     * \return The current orientation of the edge
     */
    Orientation::Enum getOrientation() const;

    /*!
     * \brief Sets the orientation of the edge
     * \param newOrientation The new orientation for the edge
     */
    void setOrientation(Orientation::Enum newOrientation);

    /*!
     * \brief Checks if a point is on the edge
     * \param p The point to check
     * \return True if the point is on the edge, false otherwise
     */
    bool contains(const QPoint& p) const;

    /*!
     * \brief Checks if two edges are equal
     * \param e1 The first edge to compare
     * \param e2 The second edge to compare
     * \return True if the edges are equal, false otherwise
     */
    friend bool operator==(const Edge& e1, const Edge& e2);


    // new functionality
    Orientation::Enum getSoftOrientation() const;
    void setSoftOrientation(Orientation::Enum newOrientation);

private:
    Orientation::Enum m_orient;
    Orientation::Enum m_softOrient;

    /*!
     * \brief Gets the middle point of the edge
     * \return The middle point of the edge
     */
    QPoint getMiddle() const;
};

