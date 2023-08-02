#pragma once

#include "point3d.h"
#include "hit.h"
#include "color.h"
#include "node.h"
#include <memory>
#include <vector>
#include <optional>

class Ray;
class Shape;

class World {
public:
    World() {};
    void add(const std::shared_ptr<Shape> sphere);
    std::optional<Hit> find_nearest(const Ray& ray, int curr_depth) const;
    void construct_tree();

    std::vector<std::shared_ptr<Shape>> objects;
    std::shared_ptr<Node> root;
};
