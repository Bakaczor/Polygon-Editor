#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <memory>
using std::make_shared;
using std::make_unique;

#include "Painter.h"

Painter::Painter(QObject *parent)
{
    image = make_shared<QImage>(m_width, m_height, QImage::Format_ARGB32);
    painter = make_unique<QPainter>(image);
}

inline QPixmap Painter::getScene() const
{
    return QPixmap::fromImage(*image);
}

