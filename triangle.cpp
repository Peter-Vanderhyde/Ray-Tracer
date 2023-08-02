#include "triangle.h"
#include "ray.h"
#include "constants.h"
#include "hit.h"
#include "material.h"
#include "texture.h"
#include "point2d.h"

Triangle::Triangle(const Point3D &vertex0, const Point3D &vertex1, const Point3D &vertex2,
                    std::shared_ptr<PropertyMap> property_map, Normal* normal_map)
    :Shape{property_map, normal_map}, vertex0{vertex0}, vertex1{vertex1}, vertex2{vertex2},
    edge1{vertex1 - vertex0}, edge2{vertex2 - vertex0} {
    Vector3D norm = cross(edge1, edge2);
    if (length(norm) < Constants::Epsilon) {
        throw std::runtime_error("Invalid triangle arrangement");
    }
    else {
        normal = unit(norm);
    }
}

void Triangle::intersect(const Ray& ray, std::optional<std::pair<const Shape*, double>>& intersected) const {
    // Moeller-Trumbore intersection
    Vector3D h = cross(ray.direction, edge2);
    double a = dot(edge1, h);
    if (a > -Constants::Epsilon && a < Constants::Epsilon) {
        return; // too close or parallel
    }

    double f = 1.0 / a;
    Vector3D s = ray.origin - vertex0;
    double u = f * dot(s, h);
    if (u <= 0.0 || u > 1.0) {
        return; // ray is outside of u: [0, 1]
    }

    Vector3D q = cross(s, edge1);
    double v = f * dot(ray.direction, q);
    if (v < 0.0 || u + v > 1.0) {
        return;
    }

    double t = f * dot(edge2, q);
    if (t > Constants::Epsilon) {
        intersected = std::make_pair(this, t);
        return;
    }
    else {
        return;
    }
}

Hit Triangle::construct_hit(const Ray& ray, double time) const {
    Point3D position = ray.at(time);
    bool negative = std::signbit(dot(ray.direction, normal));
    if (negative) {
        return Hit{time, position, normal, this};
    }
    else {
        return Hit{time, position, -normal, this};
    }
}

Point2D Triangle::uv(const Hit* hit) const {
    Vector3D P = hit->position - vertex0;
    double u = dot(P, edge1) / length(edge1);
    double v = dot(P, edge2) / length(edge2);
    double mag = (u + v) / 2.0;
    return {u / mag, v / mag};
}

Bounds Triangle::bounding_box() const {
    std::vector<double> min{vertex0.x, vertex0.y, vertex0.z};
    std::vector<double> max{vertex0.x, vertex0.y, vertex0.z};
    for (auto vertex : std::vector<Point3D>{vertex1, vertex2}) {
        for (int i = 0; i < 3; i++) {
            auto vals{vertex.val_list()};
            if (vals[i] < min[i]) {min[i] = vals[i];}
            if (vals[i] > max[i]) {max[i] = vals[i];}
        }
    }

    Vector3D tiny_vector{Constants::Epsilon, Constants::Epsilon, Constants::Epsilon};
    return Bounds(Point3D(min[0], min[1], min[2]) - tiny_vector, Point3D(max[0], max[1], max[2]) + tiny_vector);
}