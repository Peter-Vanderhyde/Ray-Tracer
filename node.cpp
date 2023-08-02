#include "node.h"
#include "random.h"
#include "hit.h"


Node::Node(const std::vector<std::shared_ptr<Shape>>& shapes, size_t start, size_t end) {
    auto objects = shapes; // Create a modifiable array of the source scene objects

    int axis = random_int(0,2);
    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;

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

        auto mid = start + object_span / 2;
        left = std::make_shared<Node>(objects, start, mid);
        right = std::make_shared<Node>(objects, mid, end);
    }

    Bounds box_left = left->bounding_box();
    Bounds box_right = right->bounding_box();

    box = surrounding_box(box_left, box_right);
}

Bounds Node::bounding_box() const {
    return box;
}

// Just here because they are virtual
Hit Node::construct_hit(const Ray&, double) const {
    return Hit(0, Point3D(0, 0, 0), Vector3D(0, 0, 0), this);
}

// Just here because they are virtual
Point2D Node::uv(const Hit*) const {
    return Point2D(0, 0);
}

void Node::intersect(const Ray& r, std::optional<std::pair<const Shape*, double>>& intersected) const {
    if (!box.intersect(r)) {
        return;
    }

    left->intersect(r, intersected);
    right->intersect(r, intersected);
}

inline bool box_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b, int axis) {
    Bounds box_a = a->bounding_box();
    Bounds box_b = b->bounding_box();

    return box_a.min().val_list()[axis] < box_b.min().val_list()[axis];
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