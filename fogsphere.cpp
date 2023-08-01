#include "fogsphere.h"
#include "constants.h"
#include "ray.h"
#include "shape.h"
#include "hit.h"
#include <iomanip>
#include "material.h"
#include "point2d.h"
#include "random.h"

FogSphere::FogSphere(const Point3D& center, double radius, double density,
            std::shared_ptr<PropertyMap> property_map, Normal* normal_map)
    :Shape{property_map, normal_map}, center{center}, radius{radius}, neg_inv_density{-1/density} {}

std::optional<double> FogSphere::intersect(const Ray& ray) const {
    // intersection of line and 3D sphere
    Vector3D oc = center - ray.origin;
    double R = dot(ray.direction, oc);
    double h2 = radius * radius - dot(oc, oc) + R * R;
    if (h2 < 0) {
        return {};
        // no hit, return {}; std::nullopt;
    }
    double h = std::sqrt(h2);
    double out_t, in_t;
    int found = 0;
    if ((R - h) > Constants::Epsilon) {
        // intersection from outside => t = R - h is closer
        out_t = R - h;
        if ((R + h) > Constants::Epsilon) {
            in_t = R + h;
            found = 2;
        }
    }
    else if ((R + h) > Constants::Epsilon) {
        in_t = R + h;
        found = 1;
    }
    else { 
        return {};
    }

    double min_t, max_t;
    if (found == 2){
        min_t = out_t;
        max_t = in_t;
        if (in_t < out_t) {
            std::swap(min_t, max_t);
        }
    }
    else if (found == 1) {
        min_t = 0;
        max_t = in_t;
    }
    else {
        return {};
    }
    const auto distance = max_t - min_t;
    const auto hit_distance = neg_inv_density * log(random_double());
    // std::cout << distance << " " << hit_distance << '\n';

    if (hit_distance > distance) {
        return {};
    }

    return min_t + hit_distance;
}

Hit FogSphere::construct_hit(const Ray& ray, double time) const {
    // calculate surface normal
    Point3D point = ray.at(time);

    Vector3D normal = unit(-ray.direction);

    return Hit{time, point, normal, this};

}

Point2D FogSphere::uv(const Hit*) const {
    return {0.0, 0.0};
}