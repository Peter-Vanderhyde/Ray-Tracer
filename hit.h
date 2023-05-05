#pragma once
#include "point3d.h"
#include "shape.h"
#include <memory>

class Vector3D;
class Object;
class Sphere;
class Material;
class Texture;

class Hit {
public:
    Hit(double time, const Point3D& position,
    const Vector3D& normal, const Shape* shape);

    double time{0};
    Point3D position;
    Vector3D normal; // (0 0 0)
    const Shape* shape;
};

// operator<<