#include <QImage>
#include <QPainter>

#include "SceneManager.h"
#include "Functions.h"

Algorithm SceneManager::s_type = Algorithm::Library;

SceneManager::SceneManager(QObject *parent) :
    QQuickImageProvider(QQuickImageProvider::Image),
    m_isBuilding(false),
    m_isDragging(false),
    lastPosition(0, 0),
    size(QSize(600, 600)),
    polyline(QList<Vertex>()),
    polygons(QList<Polygon>()),
    currVerIdx(-1),
    currEdgIdx(-1),
    currPolIdx(-1),
    currObject(Geometry::None)
{
    m_image.reset(new QImage(size, QImage::Format_ARGB32));
    m_image->fill(m_background);
    painter = QSharedPointer<QPainter>(new QPainter(m_image.get()));
}

SceneManager::~SceneManager()
{
    // QPainter must be freed before QImage
    painter.clear();
}

QImage SceneManager::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return *m_image.get();
}

void SceneManager::paint()
{
    //refresh image
    m_image->fill(m_background);

    // draw the polyline
    if (!polyline.empty())
    {
        polyline.at(0).paint(painter);
        for (int i = 1; i < polyline.count(); i++)
        {
            const Vertex& v1 = polyline.at(i - 1);
            const Vertex& v2 = polyline.at(i);
            drawLine(painter, QPoint(v1.X, v1.Y), QPoint(v2.X, v2.Y),
                     s_type, 2, QColor(0, 0, 0, 255));
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
    paint();
    drawLine(painter, lastPosition, QPoint(x, y),
             s_type, 2, QColor(0, 0, 0, 255));
    emit imageChanged();
}

void SceneManager::addVertex(int x, int y)
{
    lastPosition = QPoint(x, y);

    Vertex v(x, y);
    if (polyline.count() >= 3 && v == polyline.at(0))
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

void SceneManager::stopBuilding(int x, int y)
{
    if (polyline.isEmpty()) { return; }
    m_isBuilding = false;
    polyline.clear();

    paint();
    emit imageChanged();
}

// dodać odzielne handlery, przerobić pressed na dragging dla czytelności

void SceneManager::checkObjects(int x, int y)
{
    lastPosition = QPoint(x, y);

    if (polygons.isEmpty()) { return; }
    unselectObjects();

    // check polygons - parallelize
    for (int i = 0; i < polygons.count(); i++)
    {
        Polygon& p = polygons[i];

        // check vertices - parallelize
        int vIdx = p.checkVertices(lastPosition);
        if (vIdx != -1)
        {
            currObject = Geometry::Vertex;
            currPolIdx = i;
            currVerIdx = vIdx;
            polygons[currPolIdx].vertices[currVerIdx]->select();
            break;
        }
        else
        {
            // check edges - parallelize
            int eIdx = p.checkEdges(lastPosition);
            if (eIdx != -1)
            {
                currObject = Geometry::Edge;
                currPolIdx = i;
                currEdgIdx = eIdx;
                polygons[currPolIdx].edges[currEdgIdx].select();
                break;
            }
            else
            {
                if (p.contains(lastPosition))
                {
                    currObject = Geometry::Polygon;
                    currPolIdx = i;
                    polygons[currPolIdx].select();
                    break;
                }
            }
        }
    }

    paint();
    emit imageChanged();
}

void SceneManager::moveObject(int x, int y)
{
    int dx = x - lastPosition.x();
    int dy = y - lastPosition.y();
    lastPosition = QPoint(x, y);

    switch (currObject)
    {
        case Geometry::Polygon:
        {
            polygons[currPolIdx].drag(dx, dy);
            break;
        }
        case Geometry::Edge:
        {
            polygons[currPolIdx].edges[currEdgIdx].drag(dx, dy);
            break;
        }
        case Geometry::Vertex:
        {
            polygons[currPolIdx].vertices[currVerIdx]->drag(x, y);
            break;
        }
        case Geometry::None: return;
    }

    paint();
    emit imageChanged();
}

void SceneManager::startDragging(int x, int y)
{
    lastPosition = QPoint(x, y);
    m_isDragging = true;
}

void SceneManager::stopDragging(int x, int y)
{
    lastPosition = QPoint(x, y);
    m_isDragging = false;
}

void SceneManager::insertVertex(int x, int y)
{
    if (currPolIdx == -1 || currEdgIdx == -1) { return; }
    polygons[currPolIdx].insertVertex(x, y, currEdgIdx);

    paint();
    emit imageChanged();
}

void SceneManager::removeSelected()
{
    if (currPolIdx == -1) { return; }

    switch (currObject)
    {
        case Geometry::Polygon:
        {
            polygons.removeAt(currPolIdx);
            currObject = Geometry::None;
            currPolIdx = -1;
            paint();
            emit imageChanged();
            return;
        }
        case Geometry::Vertex:
        {
            if (currVerIdx == -1) { return; }
            if (polygons[currPolIdx].removeVertex(currVerIdx))
            {
                currObject = Geometry::None;
                currPolIdx = -1;
                currVerIdx = -1;
                paint();
                emit imageChanged();
            }
            return;
        }
        case Geometry::Edge: return;
        case Geometry::None: return;
    }
}

void SceneManager::unselectObjects()
{
    switch (currObject)
    {
        case Geometry::Polygon:
        {
            polygons[currPolIdx].unselect();
            currPolIdx = -1;
            break;
        }
        case Geometry::Edge:
        {
            polygons[currPolIdx].edges[currEdgIdx].unselect();
            currEdgIdx = -1;
            currPolIdx = -1;
            break;
        }
        case Geometry::Vertex:
        {
            polygons[currPolIdx].vertices[currVerIdx]->unselect();
            currVerIdx = -1;
            currPolIdx = -1;
            break;
        }
        case Geometry::None: return;
    }
    currObject = Geometry::None;
}

bool SceneManager::isBuilding() const
{
    return m_isBuilding;
}

bool SceneManager::isDragging() const
{
    return m_isDragging;
}
