#include "world.h"
#include "sphere.h"
#include "ray.h"
#include "constants.h"

void World::add(Point3D center, double radius) {
    objects.push_back(std::make_shared<Sphere>(center, radius));
}

std::optional<Hit> World::find_nearest(const Ray &ray) const {
    double time{Constants::Infinity};
    std::shared_ptr<Sphere> nearest;
    for (auto object : objects) {
        auto t{object->intersect(ray)};
        if (t.has_value() && t.value() < time) {
            time = t.value();
            nearest = object;
        }
    }

    if (nearest) {
        return nearest->construct_hit(ray, time);
    }
    else {
        return {};
    }
}