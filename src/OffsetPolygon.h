#pragma once

#include <QPainter>

#include "Enums.h"

/*!
 * \brief The OffsetPolygon class
 * This class represents an offset polygon, with properties such as vertices, loops, and offset.
 */
class OffsetPolygon
{
    public:
        const static uint s_pointError = 2;
        const static uint s_crossError = 1000;

        /*!
         * \brief Constructs an OffsetPolygon object
         * \param points A vector of points that define the polygon
         * \param offset The offset of the polygon
         */
        OffsetPolygon(const QVector<QPoint>& points = {}, const int& offset = 0);

        /*!
         * \brief Paints the offset polygon on a canvas
         * \param painter A shared pointer to a QPainter object
         * \param type The algorithm to use for painting the edges of the polygon
         */
        void paint(QSharedPointer<QPainter> painter, const Algorithm::Enum& type) const;

        /*!
         * \brief Updates the offset polygon
         * \param points A vector of points that define the polygon
         * \param offset The offset of the polygon
         */
        void update(const QVector<QPoint>& points, const int& offset);

        /*!
         * \brief Checks if the offset polygon is empty
         * \return True if the polygon is empty, false otherwise
         */
        inline bool empty() const { return m_polygon.empty(); }

    private:
        constexpr static QColor s_color = QColor(0, 191, 255, 255);
        const static uint s_thicc = 4;
        QVector<QVector<QPoint>> m_loops;
        QVector<QPoint> m_polygon;
};

        /*!
         * \brief Calculates the intersections of the offset polygon
         * \param points A vector of points that define the polygon
         * \param offset The offset of the polygon
         * \return A vector of points representing the intersections
         */
QVector<QPoint> calculateIntersections(const QVector<QPoint>& points, const int& offset);

        /*!
         * \brief Adds self intersections to the offset polygon
         * \param baseIntersections A vector of points representing the base intersections
         * \return A vector of pairs of points and booleans representing the intersections (true for self, false for base)
         */
QVector<QPair<QPoint, bool>> addSelfIntersections(const QVector<QPoint>& baseIntersections);

        /*!
         * \brief Removes needless points and gets independent loops from the offset polygon
         * \param allIntersections A vector of pairs of points and booleans representing all intersections
         * \return A vector of vectors of points representing the loops
         */
QVector<QVector<QPoint>> removeNeedlessAndGetLoops(QVector<QPair<QPoint, bool>>& allIntersections);

        /*!
         * \brief Removes doubles and collinear points from the offset polygon
         * \param allIntersections A vector of pairs of points and booleans representing all intersections
         * \return A vector of points representing the main loop
         */
QVector<QPoint> removeDoublesAndCollinear(const QVector<QPair<QPoint, bool>>& allIntersections);
