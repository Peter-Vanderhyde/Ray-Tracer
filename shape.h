#pragma once

#include <optional>
#include "material.h"
#include "texture.h"
#include "point2d.h"

class Hit;
class Ray;
class AABB;

class Shape {
public:
    Shape(bool leaf);
    Shape(Material* material, Texture* texture, bool leaf);

    // return the distance if ray intersects
    virtual std::optional<double> intersect(const Ray& ray) const; // doesn't change the object
    virtual Hit construct_hit(const Ray& ray, double time) const;
    virtual Point2D uv(const Hit &hit) const;
    virtual bool bounding_box(AABB& output_box) const = 0;
    std::pair<std::optional<std::vector<const Shape*>>, int> hit(const Ray &, double, double) const;

    Material* material;
    Texture* texture;
    bool leaf;
};