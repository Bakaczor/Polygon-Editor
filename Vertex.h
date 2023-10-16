#include <QPainter>
#include <QPoint>
#include <memory>
using std::shared_ptr;

#ifndef VERTEX_H
#define VERTEX_H
#endif // VERTEX_H

#import "Edge.h"

class Vertex{
public:
    QPoint curr_pos;
    shared_ptr<Edge> first_edge;
    shared_ptr<Edge> second_edge;

    Vertex(const QPainter& painter);
    void move(QPoint new_pos);
    void paint(const QPainter& painter);
private:
    const unsigned int m_size = 5;
};
