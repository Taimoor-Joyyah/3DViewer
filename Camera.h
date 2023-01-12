//
// Created by J-Bros on 1/10/2023.
//

#ifndef INC_3D_CAMERA_H
#define INC_3D_CAMERA_H

class CameraView {
public:
    float xAngle{};
    float yAngle{};
    float zAngle{};

    CameraView(float xAngle, float yAngle, float zAngle) : xAngle(xAngle), yAngle(yAngle), zAngle(zAngle) {}
};

#endif //INC_3D_CAMERA_H
