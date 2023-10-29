#pragma once
#include <QPainter>
#include <QObject>

#include "Polygon.h"

class SceneManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isBuilding READ isBuilding NOTIFY isBuildingChanged)
    Q_PROPERTY(bool isDragging READ isDragging NOTIFY isDraggingChanged)
    Q_PROPERTY(Algorithm::Enum type READ type WRITE setType NOTIFY typeChanged)
    public:
        QSharedPointer<QPainter> painter;
        QSharedPointer<QImage> image;

        QPoint lastPosition;
        QSize size;

        QList<Vertex> polyline;
        QList<Polygon> polygons;

        Geometry currObject;
        int currVerIdx;
        int currEdgIdx;
        int currPolIdx;

        explicit SceneManager(QObject* parent = nullptr);
        void paint();

        bool isBuilding() const;
        bool isDragging() const;
        Algorithm::Enum type() const;

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
        void setType(Algorithm::Enum newType);
        void changeOrientation(Orientation::Enum orient);
        void updateOffset(uint offset);

    signals:
        void imageChanged();
        void isBuildingChanged();
        void isDraggingChanged();
        void typeChanged();
        void edgeChanged(Orientation::Enum type);

    private:
        const QColor m_background = QColor(255, 255, 255, 255);

        bool m_isBuilding;
        bool m_isDragging;
        Algorithm::Enum m_type;
};
