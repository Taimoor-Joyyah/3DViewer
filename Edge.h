//
// Created by J-Bros on 1/9/2023.
//

#ifndef INC_3D_EDGE_H
#define INC_3D_EDGE_H

#include "Vertex.h"

class Edge {
public:
    Vertex &v1;
    Vertex &v2;

    Edge(Vertex &v1, Vertex &v2) : v1(v1), v2(v2) {}
    Edge(Vertex &&v1, Vertex &&v2) : v1(v1), v2(v2) {}
};

#endif //INC_3D_EDGE_H
