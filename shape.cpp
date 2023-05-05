#include "shape.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "texture.h"
#include "aabb.h"

Shape::Shape(bool leaf)
    :leaf{leaf} {}

Shape::Shape(Material* material, Texture* texture, bool leaf)
    :material{material}, texture{texture}, leaf{leaf} {}

std::optional<double> Shape::intersect(const Ray&) const {
    throw std::runtime_error("Intersect method called without overriding the base method.");
}
Hit Shape::construct_hit(const Ray&, double) const {
    throw std::runtime_error("Construct Hit method called without overriding the base method.");
}
Point2D Shape::uv(const Hit &) const {
    throw std::runtime_error("UV method called without overriding the base method.");
}

std::pair<std::optional<std::vector<const Shape*>>, int> Shape::hit(const Ray&, double, double) const {
    return {{}, 0};
}