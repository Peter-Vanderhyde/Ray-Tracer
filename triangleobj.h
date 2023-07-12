#pragma once

#include "point3d.h"
#include "vector3d.h"
#include "point2d.h"
#include "triangle.h"
#include "material.h"
#include "texture.h"
#include <optional>

class Ray;
class Hit;

class TriangleObj : public Triangle {
public:
    TriangleObj(const Point3D &vertex0, const Point3D &vertex1, const Point3D &vertex2,
        const Point2D &vt0, const Point2D& vt1, const Point2D& vt2,
        Material* material, Texture* texture, Normal* normal_map);

    Point2D uv(const Hit* hit) const override;

    Point2D vt0 = {0, 0};
    Point2D vt1 = {0, 0};
    Point2D vt2 = {0, 0};
};