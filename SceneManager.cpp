#include <QtConcurrent/QtConcurrent>
#include <QImage>
#include <QPainter>

#include "SceneManager.h"
#include "Functions.h"

SceneManager::SceneManager(QObject* parent) :
    QObject(parent),
    m_isBuilding(false),
    m_isDragging(false),
    m_type(Algorithm::Enum::Library),
    lastPosition(0, 0),
    size(QSize(800, 800)),
    polyline(QList<Vertex>()),
    polygons(QList<Polygon>()),
    currVerIdx(-1),
    currEdgIdx(-1),
    currPolIdx(-1),
    currObject(Geometry::None)
{
    image = QSharedPointer<QImage>(new QImage(size, QImage::Format_ARGB32), [this](QImage* image) {
        // the painter must be freed before freeing the device
        painter.clear();
        delete image;
    });
    image->fill(m_background);
    painter = QSharedPointer<QPainter>(new QPainter(image.get()));
}

void SceneManager::paint()
{
    // refresh image
    image->fill(m_background);

    // draw the polyline
    if (!polyline.empty())
    {
        polyline.at(0).paint(painter);
        for (uint i = 1; i < polyline.count(); i++)
        {
            const Vertex& v1 = polyline.at(i - 1);
            const Vertex& v2 = polyline.at(i);
            drawLine(painter, QPoint(v1.X, v1.Y), QPoint(v2.X, v2.Y), m_type, 2, QColor(0, 0, 0, 255));
            v2.paint(painter);
        }
    }

    // draw polygons
    if (!polygons.empty())
    {
        for (const Polygon& p : polygons)
        {
            p.paint(painter, m_type);
        }
    }
}

void SceneManager::drawProjection(int x, int y)
{
    paint();
    drawLine(painter, lastPosition, QPoint(x, y), m_type, 2, QColor(0, 0, 0, 255));
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

void SceneManager::checkObjects(int x, int y)
{
    lastPosition = QPoint(x, y);

    if (polygons.isEmpty()) { return; }
    unselectObjects();
    emit edgeChanged(Orientation::Enum::None);

    // check polygons
    QtConcurrent::blockingMap(polygons, [this](const Polygon& p) {
        std::ptrdiff_t i = std::distance(&polygons.at(0), &p);

        // check vertices
        int vIdx = p.checkVertices(lastPosition);
        if (vIdx != -1)
        {
            currObject = Geometry::Vertex;
            currPolIdx = i;
            currVerIdx = vIdx;
            polygons[currPolIdx].vertices[currVerIdx]->select();
        }
        else
        {
            // check edges
            int eIdx = p.checkEdges(lastPosition);
            if (eIdx != -1)
            {
                currObject = Geometry::Edge;
                currPolIdx = i;
                currEdgIdx = eIdx;
                polygons[currPolIdx].edges[currEdgIdx].select();
                emit edgeChanged(polygons[currPolIdx].edges[currEdgIdx].getOrientation());
            }
            else
            {
                if (p.contains(lastPosition))
                {
                    currObject = Geometry::Polygon;
                    currPolIdx = i;
                    polygons[currPolIdx].select();
                }
            }
        }
    });

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
            polygons[currPolIdx].dragEdge(dx, dy, currEdgIdx);
            break;
        }
        case Geometry::Vertex:
        {
            polygons[currPolIdx].dragVertex(x, y, currVerIdx);
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

void SceneManager::changeOrientation(Orientation::Enum orient)
{
    if (currPolIdx == -1 || currEdgIdx == -1) { return; }
    if (orient != Orientation::Enum::None)
    {
        QList<Edge>& edges = polygons[currPolIdx].edges;
        int peIdx = currEdgIdx == 0 ? edges.count() - 1 : currEdgIdx - 1;
        int neIdx = currEdgIdx == edges.count() - 1 ? 0 : currEdgIdx + 1;
        if (edges[peIdx].getOrientation() == orient || edges[neIdx].getOrientation() == orient) { return; }
    }
    polygons[currPolIdx].edges[currEdgIdx].setOrientation(orient);
    polygons[currPolIdx].updateOffset(-1);

    paint();
    emit imageChanged();
}

void SceneManager::updateOffset(uint offset)
{
    if (currObject != Geometry::Polygon) { return; }
    polygons[currPolIdx].updateOffset(offset);

    paint();
    emit imageChanged();
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

Algorithm::Enum SceneManager::type() const
{
    return m_type;
}

void SceneManager::setType(Algorithm::Enum newType)
{
    if (m_type == newType) { return; }
    m_type = newType;

    paint();
    emit imageChanged();
}
