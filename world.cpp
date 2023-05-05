#include "world.h"

#include "constants.h"
#include "material.h"
#include "sphere.h"

void World::add(const std::shared_ptr<Sphere> sphere) {
    objects.push_back(sphere);
}

std::optional<Hit> World::find_nearest(const Ray& ray) const {
    double time = Constants::Infinity;
    std::shared_ptr<Sphere> nearest = nullptr;
    for (auto object : objects) {
        std::optional<double> t = object->intersect(ray);
        if (t.has_value() && t.value() < time) {
            nearest = object;
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
