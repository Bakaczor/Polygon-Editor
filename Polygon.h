#include <list>
using std::list;

#import "Vertex.h"
#import "Edge.h"

class Polygon {
public:
    QPoint curr_pos;

    Polygon(const QPainter& painter);
    void move(QPoint new_pos);
    void paint(const QPainter& painter);
private:
    list<Vertex> m_vertices;
    list<Edge> m_edges;
};
