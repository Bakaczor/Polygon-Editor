#include <QImage>
#include <QPainter>

#include "SceneManager.h"
#include "Functions.h"

LineType SceneManager::s_type = LineType::Library;

SceneManager::SceneManager(QObject *parent) :
    QQuickImageProvider(QQuickImageProvider::Image),
    lastPosition(0, 0),
    size(QSize(600, 600)),
    polyline(QList<Vertex>()),
    polygons(QList<Polygon>()),
    m_image(size, QImage::Format_ARGB32),
    m_isBuilding(false)
{
    m_image.fill(background);
    painter = QSharedPointer<QPainter>(new QPainter(&m_image));
}

QImage SceneManager::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return m_image;
}

void SceneManager::paint()
{
    //refresh image
    m_image.fill(background);

    // draw the polyline
    if (!polyline.empty())
    {
        polyline.at(0).paint(painter);
        for (int i = 1; i < polyline.count(); i++)
        {
            const Vertex& v1 = polyline.at(i - 1);
            const Vertex& v2 = polyline.at(i);
            drawLine(painter, QPoint(v1.x, v1.y), QPoint(v2.x, v2.y), s_type);
            v2.paint(painter);
        }
    }

    // draw polygons - parallelize
    if (!polygons.empty())
    {
        for (const Polygon& p : polygons)
        {
            p.paint(painter);
        }
    }
}

void SceneManager::drawProjection(int x, int y)
{
    if (m_isBuilding)
    {
        paint();
        drawLine(painter, lastPosition, QPoint(x, y), s_type);
        emit imageChanged();
    }
}

void SceneManager::addVertex(int x, int y)
{
    lastPosition = QPoint(x, y);
    Vertex v(x, y);
    if (!polyline.empty() && v == polyline.at(0))
    {
        Polygon p(polyline);
        polygons.append(p);
        polyline.clear();
        m_isBuilding = false;
    }
    else
    {
        polyline.append(v);
        m_isBuilding = true;
    }
    paint();
    emit imageChanged();
}
