#pragma once
#include "shape.h"


class Node : public Shape {
public:
    Node() {};
    Node(const std::vector<std::shared_ptr<Shape>>& shapes, size_t start, size_t end);

    virtual void intersect(const Ray& r, std::optional<std::pair<const Shape*, double>>& intersected) const;
    Hit construct_hit(const Ray& ray, double time) const override;
    Point2D uv(const Hit* hit) const override;
    Bounds bounding_box() const override;

    std::shared_ptr<Shape> left;
    std::shared_ptr<Shape> right;
    Bounds box;
};

bool box_x_compare (const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);
bool box_y_compare (const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);
bool box_z_compare (const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);