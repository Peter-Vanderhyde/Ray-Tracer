#pragma once
#include "vector3d.h"
#include "point3d.h"

class Sphere;

class Hit {
public:
    Hit(double time, const Point3D& position,
    const Vector3D& normal,
    const Sphere &object);

    double time{0};
    Point3D position;
    Vector3D normal; // (0 0 0)
    const Sphere &object;
};

// operator<<