#pragma once

#include "point3d.h"
#include "vector3d.h"

#include <iostream>

class Ray {
public:
    Ray(const Point3D& origin, const Vector3D& direction);
    Ray(const Point3D& origin, const Vector3D& direction, double index_ratio);
    Point3D at(double time) const;

    Point3D origin;
    Vector3D direction;
    double index_ratio;
};

std::ostream& operator<<(std::ostream& out, const Ray& ray);