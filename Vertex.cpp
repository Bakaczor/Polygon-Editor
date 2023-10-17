#include <QPoint>

#include "Vertex.h"

Vertex::Vertex(QPoint pos) : m_position(pos) {}

void Vertex::paint(QPainter& painter) {
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::red);
    painter.drawPoint(m_position);
}

QPoint Vertex::position() const {
    return m_position;
}

void Vertex::setPosition(const QPoint& pos) {
    if (m_position != pos) {
        m_position = pos;
        //emit positionChanged();
    }
}

