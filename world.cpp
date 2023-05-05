#include "world.h"

#include "constants.h"
#include "diffuse.h"
#include "sphere.h"

void World::add(Point3D center, double radius, Color color, bool emitting) {
    auto diffuse = std::make_shared<Diffuse>(color, emitting);
    objects.push_back(std::make_shared<Sphere>(center, radius, diffuse));
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
