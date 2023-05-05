#include "world.h"

#include "constants.h"
#include "material.h"
#include "sphere.h"
#include "triangle.h"
#include "shape.h"

void World::add(const std::shared_ptr<Shape> shape) {
    objects.push_back(shape);
}

std::optional<Hit> World::find_nearest(const Ray& ray) const {
    double time = Constants::Infinity;
    Shape* nearest = nullptr;
    for (std::size_t i = 0; i < objects.size(); ++i) {
        std::optional<double> t = objects.at(i)->intersect(ray);
        if (t.has_value() && t.value() < time)
        {
            nearest = objects.at(i).get();
            time = t.value();
        }
    }
    if (nearest) {
        Hit hit = nearest->construct_hit(ray, time);
        return hit;
    } else {
        return {};
    }
}
