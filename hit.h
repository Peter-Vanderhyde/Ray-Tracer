#pragma once
#include "point3d.h"
#include <memory>
#include "material.h"

class Vector3D;

class Hit {
public:
    Hit(double time, const Point3D& position,
    const Vector3D& normal,
    std::shared_ptr<Material> material);

    double time{0};
    Point3D position;
    Vector3D normal; // (0 0 0)
    std::shared_ptr<Material> material;
};

// operator<<