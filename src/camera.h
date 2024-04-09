#ifndef CAMERA_H
#define CAMERA_H

#include "image.h"

class Camera {
public:
    Vector2 position;
    Vector2 offset;

    Camera() {}
    Camera(int x, int y) { position.x = x; position.y = y; offset = Vector2(0.0f, 0.0f); }
};

#endif //CAMERA_H