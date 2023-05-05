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

class Fog : public Shape {
public:
    Fog(const Point3D& center, const Vector3D& dimensions, const Vector3D& rotations, double density,
        Material* material, Texture* texture);

    // return the distance if ray intersects
    std::optional<double> intersect(const Ray& ray) const; // doesn't change the sphere
    Hit construct_hit(const Ray& ray, double time) const;
    Point2D uv(const Hit &hit) const;
    bool bounding_box(AABB& output_box) const;

    Point3D center;
    Vector3D dimensions;
    double neg_inv_density;
    std::vector<std::shared_ptr<Triangle>> triangles;

private:
    void create_plane(Point3D c1, Point3D c2, Point3D c3,
                    Material* material, Texture* texture);
    void create_box(const Vector3D &center, const Vector3D &extents, const Vector3D &rotations,
                         Material* material, Texture* texture);
};