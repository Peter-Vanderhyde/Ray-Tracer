#pragma once

#include "point3d.h"
#include "hit.h"
#include <memory>
#include <vector>
#include <optional>

// forward declare
class Sphere;
class Ray;

class World {
public:
    void add(Point3D center, double radius);
    std::optional<Hit> find_nearest(const Ray &ray) const;

    std::vector<std::shared_ptr<Sphere>> objects;
};