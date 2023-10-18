#pragma once
#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QSize>
#include <QQuickImageProvider>

#include "Polygon.h"

class SceneManager : public QQuickImageProvider
{
    Q_OBJECT
    public:
        QPainter* painter;
        QImage* image;
        QSize size;
        QList<Polygon> polygons;

        SceneManager(QObject *parent = nullptr);
        ~SceneManager();
        QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    signals:
        void imageChanged();
};


