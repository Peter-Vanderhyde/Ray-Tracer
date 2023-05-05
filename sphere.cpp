#include "sphere.h"
#include "constants.h"
#include "ray.h"
#include <iomanip>

Sphere::Sphere(const Point3D& center, double radius, std::shared_ptr<Material> material)
    :center{center}, radius{radius}, material{material} {
    if (radius <= 0) {
        throw std::runtime_error("radius must be positive: " + std::to_string(radius));
    }
}

std::optional<double> Sphere::intersect(const Ray& ray) const {
    // intersection of line and 3D sphere
    Vector3D oc = center - ray.origin;
    double R = dot(ray.direction, oc);
    double h2 = radius * radius - dot(oc, oc) + R * R;
    if (h2 < 0) {
        return {};
        // no hit, return {}; std::nullopt;
    }
    double h = std::sqrt(h2);
    if ((R - h) > Constants::Epsilon) {
        // intersection from outside => t = R - h is closer
        return R - h;
    }
    else if ((R + h) > Constants::Epsilon) {
        // intersection inside => t = R + h
        return R + h;
    }
    else { 
        return {};
    }
}

Hit Sphere::construct_hit(const Ray& ray, double time) const {
    // calculate surface normal
    Point3D point = ray.at(time);

    // error handling

    // schaser method of calculating normals on sphere
    Vector3D normal = (point - center) / radius;
    return Hit{time, point, normal, material};

}