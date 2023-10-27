#include <QPainter>
#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider
{
    Q_OBJECT
    public:
        QSharedPointer<QPainter> painter;
        QSharedPointer<QImage> image;

        ImageProvider(QSharedPointer<QPainter> painter = nullptr, QSharedPointer<QImage> image = nullptr);
        QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
};
