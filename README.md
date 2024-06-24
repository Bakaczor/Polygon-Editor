# Polygon Editor
Polygon Editor is a simple application that allows for the creation and editing of polygons. The program is built on Qt 6.5.0, using the C++17 standard.

# Controls
This section contains information on how to use the application.

## Polygon
### Creation
To start building a polygon, double-click the **left mouse button**. This way, you can add subsequent vertices. To finish building, double-click the first added vertex. If you want to cancel building the polygon, just double-click the **right mouse button**.

### Dragging
To drag the polygon, press the **right mouse button** inside the polygon and, without releasing it, move the polygon to a different location.

### Deleting
To delete a polygon, select it by clicking inside it (it will light up red), then use the `Delete` key.

## Edge
### Dragging
To drag an edge, press the **right mouse button** on it and, without releasing it, move it to a different location.

### Changing Orientation
To change the orientation of an edge, select it by clicking (it will light up red), then choose one of the options available on the left side of the interface:
- `Horizontal` - edge will remain horizontal
- `Vertical` - edge will remain vertical
- `None` - edge is free

## Vertex
### Adding
To add a new vertex to an edge, click on the desired spot on the edge with the **middle mouse button** (wheel).

### Dragging
To drag a vertex, press the **right mouse button** on it and, without releasing it, move it to a different location.

### Deleting
To delete a vertex, select it by clicking on it (it will light up red), then use the `Delete` key.

## Offset Polygon
To activate the offset polygon, select the polygon by clicking inside it (it will light up red), then choose the offset value by dragging the slider on the left side.

# Algorithm Descriptions
## Edge Orientation
### Algorithm
1. When the user changes the edge's orientation, it checks if the edge is selected and if the adjacent edges do not already have the same orientation.
2. If the validation is positive, the edge's orientation is changed, and the difference in the coordinates of its vertices (x for `Vertical`, y for `Horizontal`) is calculated. Half of this difference is added to the first vertex and subtracted from the second vertex (resulting in both being at the arithmetic mean of the given coordinate).

### Dragging Vertices
If an edge incident to the dragged vertex has an orientation set, its non-incident vertex is also dragged, but only along one axis (OX for `Vertical`, OY for `Horizontal`).

### Dragging Edges
If an edge incident to the dragged edge has an orientation set, its non-incident vertex is also dragged, but only along one axis (OX for `Vertical`, OY for `Horizontal`).

**Exception:** The hypotenuse of a right triangle - in this case, the entire figure is moved.

## Offset Polygon
### Algorithm
1. Initially, the orientation of the polygon is checked (the cross product is calculated for the vertex with the smallest coordinates and its neighbors). If the polygon is built clockwise, the offset sign is changed to negative (convention adopted).
2. Next, the offset segments are calculated - for each pair of vertices, a perpendicular vector is calculated, which is then normalized to the appropriate length. Offset vertices are created by shifting the polygon's vertices by this vector.
3. The next step is calculating the intersections of lines created by adjacent segments - a system of two linear equations is solved using Cramer's rule.
4. Then, self-intersections are searched for - each edge is checked with all non-incident edges as follows:

    ```
    1. Let p1 and p2 be the vertices of the first segment, and p3 and p4 of the second.
    2. Denote: p = p1, q = p3, r - vector p1p2, s - vector p3p4.
    3. Let r_s = r × s, if it is zero, the vectors are parallel.
    4. If not: let t = pq × s, and u = pq × r - both divided by r_s.
    5. t and u are the ratios of vectors determining the intersection point to the lengths of the segments, so if both are in the range [0, 1], the intersection exists and is equal to t * r (or equivalently u * s).
    ```

    Such found self-intersections are sorted by distance from point p1 and inserted into the sequence of vertices.
5. In the next step, two operations are performed simultaneously:
   - Whenever a self-intersection is encountered, it is toggled whether to add base vertices to the solution or not.
   - Self-intersections are pushed onto a stack along with indices. If the top of the stack matches the current vertex, the number of "good" vertices in between is checked - if at least two, a loop is detected.
    
    Let's illustrate this with an example.

    ```
    Let (0, 1, P, 2, 3, N, 4, 5, 6, 7, 8, N, P, 9, 10, 11) represent a sequence of vertices, and N and P be self-intersections.

    Added: 0, 1
    Not added:
    State change: P

    Added: 0, 1, P
    Not added: 2, 3
    State change: N

    Added: 0, 1, P, N, 4, 5, 6, 7, 8
    Not added: 2, 3
    State change: N - loop! (N is marked for removal, additionally checking if N, P, and 9 are collinear, if so, P is marked for removal)

    Added: 0, 1, P
    Not added: 2, 3, N, 4, 5, 6, 7, 8, N
    State change: P - loop! (as above, but since only one vertex is on the stack, P is treated as part of the main loop and its first occurrence is not removed from the solution)

    Added: 0, 1, P, 9, 10, 11
    Not added: 2, 3, N, 4, 5, 6, 7, 8, N, P

    Finally, we found one loop (N, 4, 5, 6, 7, 8), and the main solution is (0, 1, P, 9, 10, 11).
    ```
6. At the end, each pair of adjacent points in the main solution is checked for duplicate values - if such a value is found (though in theory it shouldn't be), one of the vertices is removed.
7. Finally, each triplet of points p1, p2, p3 is checked for collinearity by calculating p2p1 × p2p3. If this condition is met, point p2 is removed from the final solution.

---
*Copyright © 2023 Bartosz Kaczorowski*
