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
        static LineType s_type;

        QPoint lastPosition;
        QSize size;
        QList<Vertex> polyline;
        QList<Polygon> polygons;
        QSharedPointer<Polygon> currPolygon;
        QSharedPointer<QPainter> painter;

        SceneManager(QObject *parent = nullptr);
        QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

        void paint();

    public slots:
        void drawProjection(int x, int y);
        void addVertex(int x, int y);

    signals:
        void imageChanged();

        void isPressedChanged();

    private:
        const QColor background = QColor(255, 255, 255, 255);
        QImage m_image;
        bool m_isBuilding;
};


