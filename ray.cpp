#include "ray.h"

Ray::Ray(const Point3D& origin, const Vector3D& direction)
    :origin{origin}, direction{direction}, index_ratio{1.0} {}

Ray::Ray(const Point3D& origin, const Vector3D& direction, double index_ratio)
    :origin{origin}, direction{direction}, index_ratio{index_ratio} {}

Point3D Ray::at(double time) const {
    return origin + direction * time;
}

std::ostream& operator<<(std::ostream& out, const Ray& ray) {
    return out << "Ray(origin=" << ray.origin << ", dir=" << ray.direction << ")";
}