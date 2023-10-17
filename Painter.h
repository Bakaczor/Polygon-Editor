#pragma once
#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <memory>
using std::shared_ptr;
using std::unique_ptr;

class Painter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int width READ width CONSTANT)
    Q_PROPERTY(int height READ height CONSTANT)

    public:
        unique_ptr<QPainter> painter;
        shared_ptr<QImage> image;

        Painter(QObject *parent = nullptr);
        inline int width() const { return m_width; };
        inline int height() const { return m_height; };
        QPixmap getScene() const;

    private:
        const int m_width = 600;
        const int m_height = 600;

};


