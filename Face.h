//
// Created by J-Bros on 1/9/2023.
//

#ifndef INC_3D_FACE_H
#define INC_3D_FACE_H

#include "Vertex.h"

class Face {
public:
    Vertex &v1;
    Vertex &v2;
    Vertex &v3;

    Face(Vertex &v1, Vertex &v2, Vertex &v3) : v1(v1), v2(v2), v3(v3) {}
};

#endif //INC_3D_FACE_H
