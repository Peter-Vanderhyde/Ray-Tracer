#pragma once

#include "point3d.h"
#include <optional>

class Ray;

class AABB {
    public:
        AABB() {}
        AABB(const Point3D &minimum, const Point3D &maximum);

        Point3D min() const;
        Point3D max() const;

        bool hit(const Ray &r, double t_min, double t_max) const;

        Point3D minimum, maximum;
};

AABB surrounding_box(AABB box0, AABB box1);