#pragma once

#include <QPainter>
#include <QSharedPointer>
#include <QtMath>

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

// THE REST IS UNUSED

void drawSymmetricDoubleStep(QSharedPointer<QPainter> painter, const QPoint &p1, const QPoint &p2, const int& thicc, const QColor &color);

inline void pattern1x(QSharedPointer<QPainter> painter, int& x, const int& y, const int& xi, const int& thicc, const QColor &color)
{
    x += xi;
    putPixel(painter, x, y, thicc, color);
    x += xi;
    putPixel(painter, x, y, thicc, color);
}

inline void pattern2x(QSharedPointer<QPainter> painter, int& x, int& y, const int& xi, const int& yi, const int& thicc, const QColor &color)
{
    x += xi;
    putPixel(painter, x, y, thicc, color);
    x += xi;
    y += yi;
    putPixel(painter, x, y, thicc, color);
}

inline void pattern3x(QSharedPointer<QPainter> painter, int& x, int& y, const int& xi, const int& yi, const int& thicc, const QColor &color)
{
    x += xi;
    y += yi;
    putPixel(painter, x, y, thicc, color);
    x += xi;
    putPixel(painter, x, y, thicc, color);
}

inline void pattern1y(QSharedPointer<QPainter> painter, int& y, const int& x, const int& yi, const int& thicc, const QColor &color)
{
    y += yi;
    putPixel(painter, x, y, thicc, color);
    y += yi;
    putPixel(painter, x, y, thicc, color);
}

inline void pattern2y(QSharedPointer<QPainter> painter, int& y, int& x, const int& yi, const int& xi, const int& thicc, const QColor &color)
{
    y += yi;
    putPixel(painter, x, y, thicc, color);
    y += yi;
    x += xi;
    putPixel(painter, x, y, thicc, color);
}

inline void pattern3y(QSharedPointer<QPainter> painter, int& y, int& x, const int& yi, const int& xi, const int& thicc, const QColor &color)
{
    y += yi;
    x += xi;
    putPixel(painter, x, y, thicc, color);
    y += yi;
    putPixel(painter, x, y, thicc, color);
}


void drawGuptaSproull(QSharedPointer<QPainter> painter, const QPoint& p1, const QPoint& p2, const int& thicc, const QColor& color);

inline float coverage(const int& thicc, const float& D)
{
    const int w = thicc >> 1;
    const float r = 0.5f;
    auto cov = [](const float& d, const float& r) {
        if (d >= r) { return 0.0f; }
        else { return 2 * (r - d) / r; }
        //else { return (float)(0.5f - d * qSqrt(r * r - d * d) / (M_PI * r * r) - 1 / M_PI * qAsin(d / r));  }
    };
    if (w >= r)
    {
        if (w <= D) { return cov(D - w, r); }
        else { return 1 - cov(w - D, r); }
    }
    else
    {
        if (D <= w) { return 1 - cov(w - D, r) - cov(w + D, r); }
        else if (D <= r - w) { return cov(D - w, r) - cov(D + w, r); }
        else { return cov(D - w, r); }
    }
}

inline int IntensifyPixel(QSharedPointer<QPainter> painter, const int& x, const int& y, const int& thicc, const float& D, const QColor& color)
{
    float cov;
    if ((cov = coverage(thicc, D)) > 0)
    {
        putPixel(painter, x, y, thicc, QColor(cov * color.red(), cov * color.green(), cov * color.blue()));
    }
    return cov;
}

