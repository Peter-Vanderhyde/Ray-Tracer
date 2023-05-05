#pragma once
#include "point3d.h"
#include <memory>
#include "material.h"

class Vector3D;

class Hit {
public:
    Hit(double time, const Point3D& position,
    const Vector3D& normal, Material* material);

    double time{0};
    Point3D position;
    Vector3D normal; // (0 0 0)
    Material* material;
};

// operator<<