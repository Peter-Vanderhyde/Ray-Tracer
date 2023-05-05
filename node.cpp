#include "node.h"
#include "shape.h"
#include "random.h"
#include "ray.h"
#include "hit.h"
#include "aabb.h"
#include <optional>

Node::Node(std::vector<std::shared_ptr<Shape>> objects,
        size_t start, size_t end, double time0, double time1)
    :Shape{false} {

    int axis = random_int(0,2);
    auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else {
            left = objects[start+1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        int mid = start + object_span/2;
        left = std::make_shared<Node>(objects, start, mid, time0, time1);
        right = std::make_shared<Node>(objects, mid, end, time0, time1);
    }

    AABB box_left, box_right;

    if (  !left->bounding_box(box_left)
       || !right->bounding_box(box_right)
    )
        std::cerr << "No bounding box in node constructor.\n";

    box = surrounding_box(box_left, box_right);
}

std::pair<std::optional<std::vector<const Shape*>>, int> Node::hit(const Ray& r, double t_min, double t_max) const {
    int checks = 1;
    if (!box.hit(r, t_min, t_max)) {
        return {{}, 1};
    }

    std::pair<std::optional<std::vector<const Shape*>>, int> hit_left = (left->hit(r, t_min, t_max));
    std::pair<std::optional<std::vector<const Shape*>>, int> hit_right = (right->hit(r, t_min, t_max));
    checks += hit_left.second + hit_right.second;

    std::vector<const Shape *> hits;
    if (hit_left.first.has_value()) {
        hits = hit_left.first.value();
        if (hit_right.first.has_value()) {
            for (auto shape : hit_right.first.value()) {
                hits.push_back(shape);
            }
        }
        return {hits, checks};
    }
    else if (hit_right.first.has_value()) {
        hits = hit_right.first.value();
        return {hits, checks};
    }
    else {
        return {};
    }
}

bool Node::bounding_box(AABB& output_box) const {
    output_box = box;
    return true;
}

inline bool box_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b, int axis) {
    AABB box_a;
    AABB box_b;

    if (!a->bounding_box(box_a) || !b->bounding_box(box_b))
        std::cerr << "No bounding box in node constructor.\n";

    if (axis == 0) {
        return box_a.min().x < box_b.min().x;
    }
    else if (axis == 1) {
        return box_a.min().y < box_b.min().y;
    }
    else {
        return box_a.min().z < box_b.min().z;
    }
}


bool box_x_compare (const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare (const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare (const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b) {
    return box_compare(a, b, 2);
}