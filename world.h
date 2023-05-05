#pragma once

#include "point3d.h"
#include "hit.h"
#include "color.h"
#include <memory>
#include <vector>
#include <optional>

class Ray;
class Sphere;

class World {
public:
    void add(Point3D center, double radius, Color color, bool emitting);
    std::optional<Hit> find_nearest(const Ray& ray) const;

    std::vector<std::shared_ptr<Sphere>> objects;
};
