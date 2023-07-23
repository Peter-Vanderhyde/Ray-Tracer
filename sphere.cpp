#include "sphere.h"
#include "constants.h"
#include "ray.h"
#include "shape.h"
#include "hit.h"
#include <iomanip>
#include "material.h"
#include "point2d.h"

Sphere::Sphere(const Point3D& center, double radius, Vector2D tile, Vector3D rotations,
                Material* material, Texture* texture, Normal* normal_map)
    :Shape{material, texture, normal_map}, center{center}, radius{radius}, tile{tile}, rotations{rotations} {
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
    return Hit{time, point, normal, this};

}

Point2D Sphere::uv(const Hit* hit) const {
    Hit hit_copy = *hit;
    Vector3D normal = hit_copy.normal;

    // Apply rotations to the normal vector of the copied hit object
    if (rotations.x != 0 || rotations.y != 0 || rotations.z != 0) {
        normal.rotate(rotations);
    }

    double theta = std::acos(normal.z);
    double phi = std::atan2(normal.y, normal.x);

    double u = 0.5 + phi / (2 * Constants::Pi); // [0, 1] range along median of sphere
    double v = theta / Constants::Pi; // [0, 1] range along z axis
    Point2D uv{fmod(u * tile.x, 1.0), fmod(v * tile.y, 1.0)};
    return uv;
}