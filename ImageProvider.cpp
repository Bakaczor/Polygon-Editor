#include "ImageProvider.h"

ImageProvider::ImageProvider(QSharedPointer<QPainter> painter, QSharedPointer<QImage> image) : painter(painter), image(image), QQuickImageProvider(QQuickImageProvider::Image) {}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return *image.get();
}
