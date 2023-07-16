#include "trianglebillboard.h"
#include "ray.h"
#include "constants.h"
#include "hit.h"
#include "material.h"
#include "texture.h"
#include "point2d.h"
#include "triangle.h"

TriangleBillboard::TriangleBillboard(const Point3D &vertex0, const Point3D &vertex1, const Point3D &vertex2,
    Material* material, Texture* texture, Normal* normal_map)
    :Triangle{vertex0, vertex1, vertex2, material, texture, normal_map} {}

std::optional<double> TriangleBillboard::intersect(const Ray& ray) const {
    // Moeller-Trumbore intersection
    Vector3D h = cross(ray.direction, edge2);
    double a = dot(edge1, h);
    if (a > -Constants::Epsilon && a < Constants::Epsilon) {
        return {}; // too close or parallel
    }

    double f = 1.0 / a;
    Vector3D s = ray.origin - vertex0;
    double u = f * dot(s, h);
    if (u <= 0.0 || u > 1.0) {
        return {}; // ray is outside of u: [0, 1]
    }

    Vector3D q = cross(s, edge1);
    double v = f * dot(ray.direction, q);
    if (v < 0.0 || u + v > 1.0) {
        return {};
    }

    double t = f * dot(edge2, q);
    if (t <= Constants::Epsilon) {
        return {};
    }
    bool negative = std::signbit(dot(ray.direction, normal));
    if (negative) {
        return {};
    } else {
        return t;
    }
}

Hit TriangleBillboard::construct_hit(const Ray& ray, double time) const {
    Point3D position = ray.at(time);
    return Hit{time, position, -normal, this};
}