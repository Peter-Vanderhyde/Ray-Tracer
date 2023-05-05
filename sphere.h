#pragma once
#include <optional>
#include "ray.h"
#include "point3d.h"
#include "hit.h"

class Sphere {
public:
    Sphere(const Point3D& center, double radius);

    // return the distance if ray intersects
    std::optional<double> intersect(const Ray& ray) const; // doesn't change the sphere
    Hit construct_hit(const Ray& ray, double time) const;

    Point3D center;
    double radius;
};