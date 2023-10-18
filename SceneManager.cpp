#include <QImage>
#include <QPainter>

#include "SceneManager.h"

SceneManager::SceneManager(QObject *parent) : QQuickImageProvider(QQuickImageProvider::Image)
{
    size = QSize(600, 600);
    image = new QImage(size, QImage::Format_ARGB32);
    painter = new QPainter(image);
    painter->setBackground(QBrush(QColor(255, 255, 255), Qt::SolidPattern));
}

SceneManager::~SceneManager()
{
    delete painter;
    delete image;
}

QImage SceneManager::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return *image;
}


