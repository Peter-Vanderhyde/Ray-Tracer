#include "skysphere.h"
#include "constants.h"
#include "ray.h"
#include "hit.h"
#include <iomanip>
#include "point2d.h"

Skysphere::Skysphere(Point2D tile, Texture* texture)
    : tile{tile}, texture{texture} {}

std::optional<double> Skysphere::intersect(const Ray& ray) const {
    // intersection of line and 3D sphere
    Vector3D oc = center - ray.origin;
    double R = dot(ray.direction, oc);
    double h2 = radius * radius - dot(oc, oc) + R * R;
    if (h2 < 0) {
        return {};
        // no hit, return {}; std::nullopt;
    }
    double h = std::sqrt(h2);
    if ((R + h) > Constants::Epsilon) {
        // intersection inside => t = R + h
        return R + h;
    }
    else { 
        return {};
    }
}

Point2D Skysphere::uv(const Ray& ray, double time) const {
    Point3D point = ray.at(time);

    // error handling

    // schaser method of calculating normals on sphere
    Vector3D normal = (point - center) / radius;

    double theta = std::acos(normal.z);
    double phi = std::atan2(normal.y, normal.x);

    double u = 0.5 + phi / (2 * Constants::Pi); // [0, 1] range along median of sphere
    double v = theta / Constants::Pi; // [0, 1] range along z axis
    Point2D uv{fmod(u * tile.x, 1.0), fmod(v * tile.y, 1.0)};
    return uv;
}