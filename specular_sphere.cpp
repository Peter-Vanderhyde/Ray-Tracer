#include "specular_sphere.h"
#include "constants.h"
#include "ray.h"
#include "sphere.h"
#include "hit.h"
#include <iomanip>
#include "material.h"
#include "point2d.h"

SpecularSphere::SpecularSphere(const Point3D& center, double radius, Vector2D tile, Vector3D rotations,
                std::shared_ptr<PropertyMap> property_map, Normal* normal_map)
    :Sphere{center, radius, tile, rotations, property_map, normal_map} {
}

Point2D SpecularSphere::uv(const Hit* hit) const {
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