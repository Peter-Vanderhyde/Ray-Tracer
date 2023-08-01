#include "shape.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "texture.h"
#include "normal.h"

Shape::Shape(std::shared_ptr<PropertyMap> property_map, Normal* normal_map)
    :property_map{property_map}, normal_map{normal_map} {}

Material* Shape::get_material(double u, double v) const {
    return property_map->get_material(u, v);
}

Texture* Shape::get_texture(double u, double v) const {
    return property_map->get_texture(u, v);
}