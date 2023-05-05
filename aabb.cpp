#include "aabb.h"
#include "ray.h"
#include "point3d.h"
#include <math.h>


AABB::AABB(const Point3D& minimum, const Point3D& maximum)
    :minimum{minimum}, maximum{maximum} {}

Point3D AABB::min() const {
    return minimum;
}

Point3D AABB::max() const {
    return maximum;
}

bool AABB::hit(const Ray& r, double t_min, double t_max) const {
    auto invD = 1.0f / r.direction.x;
    auto t0 = (min().x - r.origin.x) * invD;
    auto t1 = (max().x - r.origin.x) * invD;
    if (invD < 0.0f)
        std::swap(t0, t1);
    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;
    if (t_max <= t_min) {
        return false;
    }

    invD = 1.0f / r.direction.y;
    t0 = (min().y - r.origin.y) * invD;
    t1 = (max().y - r.origin.y) * invD;
    if (invD < 0.0f)
        std::swap(t0, t1);
    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;
    if (t_max <= t_min) {
        return false;
    }

    invD = 1.0f / r.direction.z;
    t0 = (min().z - r.origin.z) * invD;
    t1 = (max().z - r.origin.z) * invD;
    if (invD < 0.0f)
        std::swap(t0, t1);
    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;
    if (t_max <= t_min) {
        return false;
    }
    
    return true;
}

AABB surrounding_box(AABB box0, AABB box1) {
    Point3D small(fmin(box0.min().x, box1.min().x),
                 fmin(box0.min().y, box1.min().y),
                 fmin(box0.min().z, box1.min().z));

    Point3D big(fmax(box0.max().x, box1.max().x),
               fmax(box0.max().y, box1.max().y),
               fmax(box0.max().z, box1.max().z));

    return AABB(small,big);
}