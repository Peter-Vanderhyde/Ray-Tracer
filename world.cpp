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

std::optional<Hit> World::find_nearest(const Ray& ray, int curr_depth) const {
    double time = Constants::Infinity;
    Shape* nearest = nullptr;
    Shape* curr_obj;
    for (std::size_t i = 0; i < objects.size(); ++i)
    {
        std::optional<double> t = objects.at(i)->intersect(ray);
        if (t.has_value() && t.value() < time)
        {
            curr_obj = objects.at(i).get();
            if (curr_obj->get_material(0.0, 0.0)->name == "point_light"){
                if (curr_depth != 0) {
                    nearest = objects.at(i).get();
                    time = t.value();
                }
            }
            else if (curr_obj->get_material(0.0, 0.0)->name == "directional_light") {
                Vector3D normal = curr_obj->construct_hit(ray, t.value()).normal;
                Vector3D direction = -ray.direction;
                if (curr_obj->intersect(Ray(ray.origin, -normal)).has_value()) {
                    nearest = objects.at(i).get();
                    time = t.value();
                }
                else if (acos(dot(normal, direction) / (length(normal) * length(direction))) * 180 / Constants::Pi <= curr_obj->get_material(0.0, 0.0)->spread) {
                    nearest = objects.at(i).get();
                    time = t.value();
                }
            }
            else {
                nearest = objects.at(i).get();
                time = t.value();
            }
        }
    }
    if (nearest) {
        Hit hit = nearest->construct_hit(ray, time);
        return hit;
    } else {
        return {};
    }
}