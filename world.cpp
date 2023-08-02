#include "world.h"

#include "constants.h"
#include "material.h"
#include "sphere.h"
#include "triangle.h"
#include "shape.h"
#include <cmath>


void World::add(const std::shared_ptr<Shape> shape) {
    objects.push_back(shape);
}

std::optional<Hit> World::find_nearest(const Ray& ray, int) const {
    std::optional<std::pair<const Shape*, double>> intersected;
    root->intersect(ray, intersected);
    if (intersected.has_value()) {
        return intersected.value().first->construct_hit(ray, intersected.value().second);
    }

    return {};
}

void World::construct_tree() {
    root = std::make_shared<Node>(objects, 0, objects.size());
}