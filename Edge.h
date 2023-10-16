#ifndef EDGE_H
#define EDGE_H
#endif // EDGE_H

#import "Vertex.h"

class Edge {
public:

    QPoint curr_pos;
    shared_ptr<Vertex> first_vertex;
    shared_ptr<Vertex> second_vertex;
    enum Orientation {
        Free, Vertical, Horizontal
    };

};

