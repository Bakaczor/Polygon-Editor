#pragma once
#include <QPainter>
#include <QSize>
#include <QQuickImageProvider>

#include "Polygon.h"

class SceneManager : public QQuickImageProvider
{
    Q_OBJECT
    Q_PROPERTY(bool isBuilding READ isBuilding NOTIFY isBuildingChanged)
    Q_PROPERTY(bool isPressed READ isPressed NOTIFY isPressedChanged)
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
        int currPolIdx;

        SceneManager(QObject *parent = nullptr);
        ~SceneManager();
        QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

        void paint();

        bool isBuilding() const;

        bool isPressed() const;

    public slots:
        void drawProjection(int x, int y);
        void addVertex(int x, int y);
        void stopBuilding(int x, int y);
        void checkObjects(int x, int y);
        void moveObject(int x, int y);
        void release(int x, int y);
        //void insertVertex();
        void removeVertex();
        void unselectObjects();

    signals:
        void imageChanged();
        void isBuildingChanged();
        void isPressedChanged();


    private:
        const QColor m_background = QColor(255, 255, 255, 255);

        bool m_isBuilding;
        bool m_isPressed;

        QScopedPointer<QImage> m_image;
};


