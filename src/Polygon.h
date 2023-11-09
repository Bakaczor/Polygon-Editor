#pragma once

#include "Edge.h"
#include "OffsetPolygon.h"

/*!
 * \brief The Polygon class
 * This class represents a polygon, with properties such as vertices, edges, and offset.
 */
class Polygon
{
    public:
        QVector<QSharedPointer<Vertex>> vertices;
        QVector<Edge> edges;

        /*!
         * \brief Constructs a Polygon object
         * \param verts A vector of vertices that define the polygon
         */
        Polygon(QVector<Vertex> verts, QVector<Orientation::Enum> orients = {});

        /*!
         * \brief Moves the polygon by a specified amount
         * \param dx The amount to move the polygon along the x-axis
         * \param dy The amount to move the polygon along the y-axis
         */
        void drag(int dx, int dy);

        /*!
         * \brief Paints the polygon on a canvas
         * \param painter A shared pointer to a QPainter object
         * \param type The algorithm to use for painting the edges of the polygon
         */
        void paint(QSharedPointer<QPainter> painter, const Algorithm::Enum& type) const;

        /*!
         * \brief Selects the polygon
         * This method changes the color and thickness of the vertices and edges to indicate that the polygon is selected.
         */
        void select();

        /*!
         * \brief Unselects the polygon
         * This method changes the color and thickness of the vertices and edges to indicate that the polygon is not selected.
         */
        void unselect();

        /*!
         * \brief Inserts a vertex into the polygon
         * \param x The x-coordinate of the new vertex
         * \param y The y-coordinate of the new vertex
         * \param eIdx The index of the edge at which to insert the new vertex
         */
        void insertVertex(int x, int y, int eIdx);

        /*!
         * \brief Removes a vertex from the polygon
         * \param vIdx The index of the vertex to remove
         * \return True if the vertex was removed, false otherwise
         */
        bool removeVertex(int vIdx);

        /*!
         * \brief Checks if the polygon contains a specified point
         * \param p The point to check
         * \return True if the polygon contains the point, false otherwise
         */
        bool contains(const QPoint& p) const;

        /*!
         * \brief Checks which vertex of the polygon is at a specified point
         * \param p The point to check
         * \return The index of the vertex at the point, or -1 if no vertex is at the point
         */
        int checkVertices(const QPoint& p) const;

        /*!
         * \brief Checks which edge of the polygon contains a specified point
         * \param p The point to check
         * \return The index of the edge containing the point, or -1 if no edge contains the point
         */
        int checkEdges(const QPoint& p) const;

        /*!
         * \brief Moves a vertex of the polygon to a specified position
         * \param x The new x-coordinate of the vertex
         * \param y The new y-coordinate of the vertex
         * \param currVerIdx The index of the vertex to move
         */
        void dragVertex(int x, int y, int currVerIdx);

        /*!
         * \brief Moves an edge of the polygon by a specified amount
         * \param dx The amount to move the edge along the x-axis
         * \param dy The amount to move the edge along the y-axis
         * \param currEdgIdx The index of the edge to move
         */
        void dragEdge(int dx, int dy, int currEdgIdx);

        /*!
         * \brief Updates the offset of the polygon
         * \param offset The new offset
         */
        void updateOffset(int offset = 101);

    private:
        int m_offset;
        OffsetPolygon m_offsetPoly;
};
