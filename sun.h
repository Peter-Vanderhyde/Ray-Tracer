#pragma once
#include "vector3d.h"
#include "color.h"

class Sun {
public:
    Sun(Vector3D direction, Color color, double intensity);

    Vector3D direction;
    Color color;
    double intensity;
};