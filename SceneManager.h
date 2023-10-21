#pragma once
#include <QPainter>
#include <QSize>
#include <QQuickImageProvider>
#include <QSharedPointer>

#include "Polygon.h"

class SceneManager : public QQuickImageProvider
{
    Q_OBJECT
    public:
        static Algorithm s_type;

        QSharedPointer<QPainter> painter;

        QPoint lastPosition;
        QSize size;

        QList<Vertex> polyline;
        QList<Polygon> polygons;

        Geometry currObject;
        Vertex* currVertex;
        Edge* currEdge;
        Polygon* currPolygon;

        SceneManager(QObject *parent = nullptr);
        ~SceneManager();
        QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

        void paint();

    public slots:
        void drawProjection(int x, int y);
        void addVertex(int x, int y);
        void stopBuilding(int x, int y);
        void checkObjects(int x, int y);
        void todo(int x, int y);

    signals:
        void imageChanged();

    private:
        const QColor m_background = QColor(255, 255, 255, 255);

        bool m_isBuilding;
        bool m_isPressed;

        QImage m_image;
};


