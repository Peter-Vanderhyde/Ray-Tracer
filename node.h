#pragma once

#include "shape.h"
#include "aabb.h"
#include <optional>

class Ray;
class Hit;

class Node : public Shape {
public:
    Node();
    Node(std::vector<std::shared_ptr<Shape>> src_objects,
        size_t start, size_t end, double time0, double time1);
    
    std::pair<std::optional<std::vector<const Shape*>>, int> hit(const Ray& r, double t_min, double t_max) const;
    virtual bool bounding_box(AABB& output_box) const override;

    std::shared_ptr<Shape> left;
    std::shared_ptr<Shape> right;
    AABB box;
};

inline bool box_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b, int axis);

bool box_x_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);

bool box_y_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);

bool box_z_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);