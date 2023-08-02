#pragma once

#include "point3d.h"
#include "vector3d.h"
#include "point2d.h"
#include "shape.h"
#include "material.h"
#include "texture.h"
#include <optional>

class Ray;
class Hit;

class Triangle : public Shape {
public:
    Triangle(const Point3D &vertex0, const Point3D &vertex1, const Point3D &vertex2,
                std::shared_ptr<PropertyMap> property_map, Normal* normal_map);


    virtual void intersect(const Ray& ray, std::optional<std::pair<const Shape*, double>>& intersected) const override;
    Hit construct_hit(const Ray& ray, double time) const override;
    Point2D uv(const Hit* hit) const override;
    Bounds bounding_box() const override;

    Point3D vertex0, vertex1, vertex2;
    Vector3D edge1, edge2, normal;
};