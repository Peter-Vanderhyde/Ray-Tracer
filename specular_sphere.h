#pragma once
#include <optional>
#include "point3d.h"
#include "hit.h"
#include "material.h"
#include "sphere.h"
#include "point2d.h"
#include <memory>

class Ray;

class SpecularSphere : public Sphere{
public:
    SpecularSphere(const Point3D& center, double radius, Vector2D tile, Vector3D rotations,
            std::shared_ptr<PropertyMap> property_map, Normal* normal_map);

    Point2D uv(const Hit* hit) const override;

    Point3D center;
    double radius;
    Vector2D tile;
    Vector3D rotations{0, 0, 0};
    Texture* second_texture;
};