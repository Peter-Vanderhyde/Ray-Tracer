#pragma once

#include <optional>
#include "point3d.h"
#include "hit.h"
#include "material.h"
#include "shape.h"
#include "point2d.h"
#include "vector3d.h"
#include "triangle.h"
#include <memory>

class Ray;

class FogSphere : public Shape{
public:
    FogSphere(const Point3D& center, double radius, double density,
        std::shared_ptr<PropertyMap> property_map, Normal* normal_map);

    // return the distance if ray intersects
    void intersect(const Ray& ray, std::optional<std::pair<const Shape*, double>>& intersected) const override; // doesn't change the sphere
    Hit construct_hit(const Ray& ray, double time) const override;
    Point2D uv(const Hit* hit) const override;
    Bounds bounding_box() const override;

    Point3D center;
    double radius, neg_inv_density;
};