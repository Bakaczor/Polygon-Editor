#include <QPoint>
#include <QtMath>

#include <optional>

/*!
 * \brief Calculates the dot product of two 2D points
 * \param p1 The first point
 * \param p2 The second point
 * \return The dot product of the two points
 */
inline long dot2D(const QPoint& p1, const QPoint& p2)
{
    return p1.x() * p2.x() + p1.y() * p2.y();
}

/*!
 * \brief Calculates the cross product of two 2D points
 * \param p1 The first point
 * \param p2 The second point
 * \return The cross product of the two points
 */
inline long cross2D(const QPoint& p1, const QPoint& p2)
{
    return p1.x() * p2.y() - p1.y() * p2.x();
}

/*!
 * \brief Finds the intersection point of two segments
 * \param p1 The starting point of the first segment
 * \param p2 The ending point of the first segment
 * \param p3 The starting point of the second segment
 * \param p4 The ending point of the second segment
 * \return The intersection point of the two segments, or std::nullopt if the segments do not intersect
 */
std::optional<QPoint> segmentIntersection(const QPoint& p1, const QPoint& p2, const QPoint& p3, const QPoint& p4);

/*!
 * \brief Finds the intersection point of two lines
 * \param p1 The first point of the first line
 * \param p2 The second point of the first line
 * \param p3 The first point of the second line
 * \param p4 The seond point of the second line
 * \return The intersection point of the two lines, or std::nullopt if the lines do not intersect
 */
std::optional<QPoint> lineIntersection(const QPoint& p1, const QPoint& p2, const QPoint& p3, const QPoint& p4);


/*!
 * \brief Offsets a line segment by a specified amount
 * \param p1 The starting point of the line segment
 * \param p2 The ending point of the line segment
 * \param offset The amount to offset the line segment
 * \return A pair of points representing the offset line segment
 */
QPair<QPoint, QPoint> offsetSegment(const QPoint& p1, const QPoint& p2, const int& offset);

/*!
 * \brief Checks the winding order of a polygon
 * \param points A vector of points that define the polygon
 * \return True if the polygon's winding order is counter-clockwise, false otherwise
 */
bool polygonSign(const QVector<QPoint>& points);

// not used
int findSector(const QPoint& p1, const QPoint& p2);

// not used
QPoint adjustDistance(const QPoint& P, const QPoint& oP, const int& offset);
