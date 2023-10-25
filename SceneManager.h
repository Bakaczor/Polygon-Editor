#pragma once
#include <QPainter>
#include <QSize>
#include <QQuickImageProvider>

#include "Polygon.h"

class SceneManager : public QQuickImageProvider
{
    Q_OBJECT
    Q_PROPERTY(bool isBuilding READ isBuilding NOTIFY isBuildingChanged)
    Q_PROPERTY(bool isDragging READ isDragging NOTIFY isDraggingChanged)
    public:
        static Algorithm s_type;

        QSharedPointer<QPainter> painter;

        QPoint lastPosition;
        QSize size;

        QList<Vertex> polyline;
        QList<Polygon> polygons;

        Geometry currObject;
        int currVerIdx;
        int currEdgIdx;
        int currPolIdx;

        SceneManager(QObject *parent = nullptr);
        ~SceneManager();
        QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

        void paint();

        bool isBuilding() const;

        bool isDragging() const;

    public slots:
        void drawProjection(int x, int y);
        void addVertex(int x, int y);
        void stopBuilding(int x, int y);
        void checkObjects(int x, int y);
        void startDragging(int x, int y);
        void moveObject(int x, int y);
        void stopDragging(int x, int y);
        void insertVertex(int x, int y);
        void removeSelected();
        void unselectObjects();

    signals:
        void imageChanged();
        void isBuildingChanged();
        void isDraggingChanged();


    private:
        const QColor m_background = QColor(255, 255, 255, 255);

        bool m_isBuilding;
        bool m_isDragging;

        QScopedPointer<QImage> m_image;
};


