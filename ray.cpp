#include "ray.h"

Ray::Ray(const Point3D& origin, const Vector3D& direction)
    :origin{origin}, direction{direction} {}

Point3D Ray::at(double time) const {
    return origin + direction * time;
}

std::ostream& operator<<(std::ostream& out, const Ray& ray) {
    return out << "Ray(origin=" << ray.origin << ", dir=" << ray.direction << ")";
}