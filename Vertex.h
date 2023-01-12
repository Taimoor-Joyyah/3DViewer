//
// Created by J-Bros on 1/9/2023.
//

#ifndef INC_3D_VERTEX_H
#define INC_3D_VERTEX_H

#include "raylib.h"


class Vertex {
public:
    Vector3 vertex{};

    Vertex(float x, float y, float z) {
        vertex = {x,y,z};
    }
};


#endif //INC_3D_VERTEX_H
