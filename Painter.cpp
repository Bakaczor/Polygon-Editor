#include "Painter.h"

inline QImage Painter::image() const
{
    return m_image;
}

inline void Painter::setImage(const QImage& newImage)
{
    if (m_image != newImage) {
        m_image = newImage;
        emit imageChanged();
    }
}
