#pragma once

#include "point3d.h"
#include "vector3d.h"
#include "point2d.h"
#include "triangle.h"
#include "material.h"
#include "texture.h"
#include <optional>

class TriangleBillboard : public Triangle {
public:
    TriangleBillboard(const Point3D &vertex0, const Point3D &vertex1, const Point3D &vertex2,
                        std::shared_ptr<PropertyMap> property_map, Normal* normal_map);

    void intersect(const Ray& ray, std::optional<std::pair<const Shape*, double>>& intersected) const override;
    Hit construct_hit(const Ray& ray, double time) const override;
};