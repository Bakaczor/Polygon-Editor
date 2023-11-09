#include <QtConcurrent/QtConcurrent>
#include <QImage>

#include "SceneManager.h"
#include "Functions.h"
#include "Icon.h"

SceneManager::SceneManager(QObject* parent) :
    QObject(parent),
    m_isBuilding(false),
    m_isDragging(false),
    m_automaticOrientation(false),
    m_type(Algorithm::Enum::Library),
    lastPosition(0, 0),
    size(QSize(800, 800)),
    polyline(QVector<Vertex>()),
    polygons(QVector<Polygon>()),
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
    image->fill(s_background);
    painter = QSharedPointer<QPainter>(new QPainter(image.get()));
}

void SceneManager::paint()
{
    // refresh image
    image->fill(s_background);

    // draw the polyline
    if (!polyline.empty())
    {
        polyline.at(0).paint(painter);
        for (uint i = 1; i < polyline.count(); i++)
        {
            const Vertex& v1 = polyline.at(i - 1);
            const Vertex& v2 = polyline.at(i);
            drawLine(painter, QPoint(v1.X, v1.Y), QPoint(v2.X, v2.Y), m_type, 2, QColor(0, 0, 0, 255));

            // new functionality
            if (m_automaticOrientation && !polylineOrientations.empty())
            {
                Icon icon;
                icon.setPosition(getMiddle(v1, v2));
                icon.setSource(polylineOrientations.at(i - 1));
                icon.paint(painter.get());
            }
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

    // new functionality
    if (m_automaticOrientation)
    {
        const int margin = 5;
        int dx = qFabs(lastPosition.x() - x);
        int dy = qFabs(lastPosition.y() - y);
        QPoint mid = getMiddle(lastPosition, QPoint(x, y));
        if (dx <= margin)
        {
            Icon icon;
            icon.setPosition(mid);
            icon.setSource(Orientation::Enum::Vertical);
            icon.paint(painter.get());
            drawLine(painter, QPoint(mid.x(), lastPosition.y()), QPoint(mid.x(), y), m_type, 2, QColor(0, 0, 0, 255));
        }
        else if (dy <= margin)
        {
            Icon icon;
            icon.setPosition(mid);
            icon.setSource(Orientation::Enum::Horizontal);
            icon.paint(painter.get());
            drawLine(painter, QPoint(lastPosition.x(), mid.y()), QPoint(x, mid.y()), m_type, 2, QColor(0, 0, 0, 255));
        }
        else
        {
            drawLine(painter, lastPosition, QPoint(x, y), m_type, 2, QColor(0, 0, 0, 255));
        }
    }
    else
    {
        drawLine(painter, lastPosition, QPoint(x, y), m_type, 2, QColor(0, 0, 0, 255));
    }
    emit imageChanged();
}

void SceneManager::addVertex(int x, int y)
{
    lastPosition = QPoint(x, y);

    Vertex v(x, y);

    // new functionality
    if (m_automaticOrientation && !polyline.empty())
    {
        const int margin = 5;
        const Vertex& pv = polyline.constLast();
        int dx = qFabs(v.X - pv.X);
        int dy = qFabs(v.Y - pv.Y);
        if (dx <= margin)
        {
            polylineOrientations.append(Orientation::Enum::Vertical);
            v.X = pv.X + (pv.X - v.X) / 2;
        }
        else if (dy <= margin)
        {
            polylineOrientations.append(Orientation::Enum::Horizontal);
            v.Y = pv.Y + (pv.Y - v.Y) / 2;
        }
        else
        {
            polylineOrientations.append(Orientation::Enum::None);
        }
    }

    if (polyline.count() >= 3 && v == polyline.at(0))
    {
        // new functionality
        if (m_automaticOrientation && !polyline.empty())
        {
            Polygon p(polyline, polylineOrientations);
            polygons.append(p);
        }
        else
        {
            Polygon p(polyline);
            polygons.append(p);
        }
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

void SceneManager::stopBuilding()
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
            // new functionality
            if (m_automaticOrientation)
            {
                const int margin = 5;
                QVector<Edge>& edges = polygons[currPolIdx].edges;

                int peIdx = currVerIdx == 0 ? edges.count() - 1 : currVerIdx - 1;
                if (edges.at(peIdx).getOrientation() == Orientation::Enum::None)
                {
                    int dx = qFabs(edges.at(peIdx).first->X - edges.at(peIdx).second->X);
                    int dy = qFabs(edges.at(peIdx).first->Y - edges.at(peIdx).second->Y);
                    if (dx <= margin)
                    {
                        edges[peIdx].setSoftOrientation(Orientation::Enum::Vertical);
                    }
                    else if (dy <= margin)
                    {
                        edges[peIdx].setSoftOrientation(Orientation::Enum::Horizontal);
                    }
                    else
                    {
                        edges[peIdx].setSoftOrientation(Orientation::Enum::None);
                    }
                }

                int neIdx = currVerIdx;
                if (edges.at(neIdx).getOrientation() == Orientation::Enum::None)
                {
                    int dx = qFabs(edges.at(neIdx).first->X - edges.at(neIdx).second->X);
                    int dy = qFabs(edges.at(neIdx).first->Y - edges.at(neIdx).second->Y);
                    if (dx <= margin)
                    {
                        edges[neIdx].setSoftOrientation(Orientation::Enum::Vertical);
                    }
                    else if (dy <= margin)
                    {
                        edges[neIdx].setSoftOrientation(Orientation::Enum::Horizontal);
                    }
                    else
                    {
                        edges[neIdx].setSoftOrientation(Orientation::Enum::None);
                    }
                }
            }
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

    // new functionality
    if (currObject == Geometry::Vertex)
    {
        if (currPolIdx != -1 && currVerIdx != -1)
        {
            bool repaint = false;
            QVector<Edge>& edges = polygons[currPolIdx].edges;

            int peIdx = currVerIdx == 0 ? edges.count() - 1 : currVerIdx - 1;
            Orientation::Enum peOrient = edges.at(peIdx).getSoftOrientation();
            if (peOrient != Orientation::Enum::None)
            {
                repaint = true;
                edges[peIdx].setOrientation(peOrient);
            }

            int neIdx = currVerIdx;
            Orientation::Enum neOrient = edges.at(neIdx).getSoftOrientation();
            if (neOrient != Orientation::Enum::None)
            {
                repaint = true;
                edges[neIdx].setOrientation(neOrient);
            }

            if (repaint)
            {
                paint();
                emit imageChanged();
            }
        }
    }
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
    polygons[currPolIdx].updateOffset();

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

void SceneManager::changeAutomaticOrientation()
{
    m_automaticOrientation = !m_automaticOrientation;
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
