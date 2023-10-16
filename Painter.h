#pragma once
#include <QObject>
#include <QImage>
#include <QPixmap>

class Painter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)

    public:
        Painter(QObject *parent = nullptr);

        QImage image() const;
        void setImage(const QImage &newImage);

    signals:
        void imageChanged();

    public slots:
        void paint();

    private:
        QImage m_image;

};


