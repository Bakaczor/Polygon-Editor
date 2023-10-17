#ifndef VERTEX_H
#define VERTEX_H
#endif // VERTEX_H

#include <QPainter>
#include <QPoint>
#include <memory>
using std::shared_ptr;

//#import "Edge.h"

class Vertex{
public:
    Q_PROPERTY(QPoint position READ position WRITE setPosition NOTIFY positionChanged)
    //shared_ptr<Edge> first_edge;
    //shared_ptr<Edge> second_edge;

    Vertex(QPoint pos);
    QPoint position() const;
    void setPosition(const QPoint& pos);
    void move(QPoint pos);
    void paint(QPainter& painter);

signals:
    void positionChanged();

private:
    const unsigned int m_size = 5;
    QPoint m_position;
};

