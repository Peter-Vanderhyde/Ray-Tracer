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
    Sphere(const Point3D& center, double radius, Vector2D tile, Vector3D rotations, Material* material, Texture* texture, Normal* normal_map);

    // return the distance if ray intersects
    std::optional<double> intersect(const Ray& ray) const override; // doesn't change the sphere
    Hit construct_hit(const Ray& ray, double time) const override;
    Point2D uv(const Hit* hit) const override;

    Point3D center;
    double radius;
    Vector2D tile;
    Vector3D rotations{0, 0, 0};
};