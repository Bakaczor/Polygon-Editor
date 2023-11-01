#include <QPainter>
#include <QSharedPointer>

#include "Enums.h"

/*!
 * \brief Draws a single pixel at the specified coordinates
 * \param painter A shared pointer to a QPainter object
 * \param x The x-coordinate of the pixel
 * \param y The y-coordinate of the pixel
 * \param thicc The thickness of the pixel
 * \param color The color of the pixel
 */
void putPixel(QSharedPointer<QPainter> painter, const int& x, const int& y, const int& thicc, const QColor& color);

/*!
 * \brief Draws a line between two points using a specified algorithm
 * \param painter A shared pointer to a QPainter object
 * \param p1 The starting point of the line
 * \param p2 The ending point of the line
 * \param type The algorithm to use for drawing the line
 * \param thicc The thickness of the line
 * \param color The color of the line
 */
void drawLine(QSharedPointer<QPainter> painter, const QPoint& p1, const QPoint& p2, const Algorithm::Enum& type, const int& thicc, const QColor& color);

/*!
 * \brief Draws a line between two points using the Bresenham's line algorithm
 * \param painter A shared pointer to a QPainter object
 * \param p1 The starting point of the line
 * \param p2 The ending point of the line
 * \param thicc The thickness of the line
 * \param color The color of the line
 */
void drawBresenham(QSharedPointer<QPainter> painter, const QPoint& p1, const QPoint& p2, const int& thicc, const QColor& color);

/*!
 * \brief Draws a line between two points using the built-in QPainter::drawLine function
 * \param painter A shared pointer to a QPainter object
 * \param p1 The starting point of the line
 * \param p2 The ending point of the line
 * \param thicc The thickness of the line
 * \param color The color of the line
 */
void drawLibrary(QSharedPointer<QPainter> painter, const QPoint& p1, const QPoint& p2, const int& thicc, const QColor& color);

/*!
 * \brief Draws a polygon filled with a specified color
 * \param painter A shared pointer to a QPainter object
 * \param points A vector of points that define the polygon
 * \param color The color to fill the polygon with
 */
void drawBack(QSharedPointer<QPainter> painter, const QVector<QPoint>& points, const QColor& color);
