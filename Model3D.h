//
// Created by J-Bros on 1/9/2023.
//

#ifndef INC_3D_MODEL3D_H
#define INC_3D_MODEL3D_H

#include <list>

#include "Vertex.h"
#include "Edge.h"
#include "Face.h"

using namespace std;

class Model3D {
public:
    Model3D(list<Vertex> &&vertices, list<Edge> &&edges, list<Face> &&faces) :
    vertices(vertices), edges(edges), faces(faces) {}

    const list <Vertex> &getVertices() const {
        return vertices;
    }

    const list <Edge> &getEdges() const {
        return edges;
    }

    const list <Face> &getFaces() const {
        return faces;
    }

private:
    list<Vertex> vertices;
    list<Edge> edges;
    list<Face> faces;
};


#endif //INC_3D_MODEL3D_H
