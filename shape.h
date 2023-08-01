#pragma once

#include <optional>
#include "material.h"
#include "texture.h"
#include "point2d.h"
#include "normal.h"
#include "property_map.h"

class Hit;
class Ray;

class Shape {
public:
    Shape(std::shared_ptr<PropertyMap> property_map, Normal* normal_map);

    // return the distance if ray intersects
    virtual std::optional<double> intersect(const Ray& ray) const = 0; // doesn't change the object
    virtual Hit construct_hit(const Ray& ray, double time) const = 0;
    virtual Point2D uv(const Hit* hit) const = 0;
    Material* get_material(double u, double v) const;
    Texture* get_texture(double u, double v) const;


    std::shared_ptr<PropertyMap> property_map;
    Normal *normal_map;
};