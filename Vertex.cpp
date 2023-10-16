#include "Vertex.h"

Vertex::Vertex(QQuickItem* parent) : QQuickPaintedItem(parent), m_dragging(false) {
    setAcceptedMouseButtons(Qt::LeftButton);
}

void Vertex::paint(QPainter* painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::red);
    painter->drawPoint(m_position);
}

void Vertex::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        event->accept();
    }
}

void Vertex::mouseMoveEvent(QMouseEvent* event) {
    if (m_dragging) {
        setX(event->pos().x());
        setY(event->pos().y());
        event->accept();
    }
}

void Vertex::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        event->accept();
    }
}

QPointF Vertex::position() const {
    return m_position;
}

void Vertex::setPosition(const QPointF& pos) {
    if (m_position != pos) {
        m_position = pos;
        emit positionChanged();
    }
}
