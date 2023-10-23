#include <QImage>
#include <QPainter>

#include "SceneManager.h"
#include "Functions.h"

Algorithm SceneManager::s_type = Algorithm::Library;

SceneManager::SceneManager(QObject *parent) :
    QQuickImageProvider(QQuickImageProvider::Image),
    m_image(size, QImage::Format_ARGB32),
    m_isBuilding(false),
    m_isPressed(false),
    lastPosition(0, 0),
    size(QSize(600, 600)),
    polyline(QList<Vertex>()),
    polygons(QList<Polygon>()),
    currVertex(nullptr),
    currEdge(nullptr),
    currPolygon(nullptr),
    currObject(Geometry::None)
{
    m_image.fill(m_background);
    painter = QSharedPointer<QPainter>(new QPainter(&m_image));
}

SceneManager::~SceneManager()
{
    // QPainter should be freed before QImage
    painter.clear();
}

QImage SceneManager::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return m_image;
}

void SceneManager::paint()
{
    //refresh image
    m_image.fill(m_background);

    // draw the polyline
    if (!polyline.empty())
    {
        polyline.at(0).paint(painter);
        for (int i = 1; i < polyline.count(); i++)
        {
            const Vertex& v1 = polyline.at(i - 1);
            const Vertex& v2 = polyline.at(i);
            drawLine(painter, QPoint(v1.X, v1.Y), QPoint(v2.X, v2.Y), s_type);
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
    drawLine(painter, lastPosition, QPoint(x, y), s_type);
    emit imageChanged();
}

void SceneManager::addVertex(int x, int y)
{
    qDebug() << "Double clicked: (" << x << "," << y << ")\n";

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
    if (polygons.isEmpty()) { return; }
    currObject = Geometry::None;
    currVertex = nullptr;
    currEdge = nullptr;
    currPolygon = nullptr;

    qDebug() << "Pressed: (" << x << "," << y << ")\n";

    lastPosition = QPoint(x, y);
    m_isPressed = true;

    // check polygons - parallelize
    for (Polygon& p : polygons)
    {
        if (p.contains(lastPosition))
        {
            // always set current Polygon
            currPolygon = &p;
            Vertex* v = p.checkVertices(lastPosition);
            if (v != nullptr)
            {
                currVertex = v;
                currObject = Geometry::Vertex;
                return;
            }

            Edge* e = p.checkEdges(lastPosition);
            if (e != nullptr)
            {
                currEdge = e;
                currObject = Geometry::Edge;
                return;
            }

            // but not always as current object
            currObject = Geometry::Polygon;
            return;
        }
    }
}

void SceneManager::moveObject(int x, int y)
{
    qDebug() << "Move object: (" << x << "," << y << ")\n";

    switch(currObject)
    {
        case Geometry::Polygon: break;
        case Geometry::Edge:break;
        case Geometry::Vertex:
        {
            currVertex->drag(x, y);
            break;
        }
        case Geometry::None: return;
    }

    currPolygon->rebuild();
    lastPosition = QPoint(x, y);
    paint();
    emit imageChanged();
}

void SceneManager::release(int x, int y)
{
    qDebug() << "Release: (" << x << "," << y << ")\n";

    lastPosition = QPoint(x, y);
    m_isPressed = false;
}

bool SceneManager::isBuilding() const
{
    return m_isBuilding;
}

bool SceneManager::isPressed() const
{
    return m_isPressed;
}
