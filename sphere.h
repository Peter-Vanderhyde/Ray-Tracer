#pragma once
#include <optional>
#include "point3d.h"
#include "hit.h"
#include "material.h"
#include "shape.h"
#include "point2d.h"
#include <memory>

class Ray;

class Sphere : public Shape{
public:
    Sphere(const Point3D& center, double radius, Vector2D tile, Vector3D rotations,
            std::shared_ptr<PropertyMap> property_map, Normal* normal_map);

    // return the distance if ray intersects
    void intersect(const Ray& ray, std::optional<std::pair<const Shape*, double>>& intersected) const override; // doesn't change the sphere
    Hit construct_hit(const Ray& ray, double time) const override;
    Point2D uv(const Hit* hit) const override;
    Bounds bounding_box() const override;

    Point3D center;
    double radius;
    Vector2D tile;
    Vector3D rotations{0, 0, 0};
};