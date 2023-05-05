#pragma once

#include "point3d.h"
#include "hit.h"
#include "color.h"
#include <memory>
#include <vector>
#include <optional>

class Ray;
class Shape;

class World {
public:
    void add(const std::shared_ptr<Shape> sphere);
    std::optional<Hit> find_nearest(const Ray& ray) const;

    std::vector<std::shared_ptr<Shape>> objects;
};
