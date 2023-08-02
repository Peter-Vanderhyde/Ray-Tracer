#include "bounds.h"
#include <vector>
#include "math.h"
#include "constants.h"


Bounds::Bounds(const Point3D& a, const Point3D& b)
    : minimum{a}, maximum{b} {}

Point3D Bounds::min() const {
    return minimum;
}

Point3D Bounds::max() const {
    return maximum;
}

bool Bounds::intersect(const Ray& r) const {
    std::vector<double> min_vals, max_vals, origin, direction;
    min_vals = minimum.val_list();
    max_vals = maximum.val_list();
    origin = r.origin.val_list();
    direction = r.direction.val_list();

    for (int i = 0; i < 3; i++) {
        auto t0 = std::min((min_vals[i] - origin[i]) / direction[i],
                            (max_vals[i] - origin[i]) / direction[i]);
        auto t1 = std::max((min_vals[i] - origin[i]) / direction[i],
                            (max_vals[i] - origin[i]) / direction[i]);
        
        if (t1 <= t0) {
            return false;
        }
    }
    return true;
}

Bounds surrounding_box(Bounds box0, Bounds box1) {
    Point3D small{std::min(box0.min().x, box1.min().x),
                  std::min(box0.min().y, box1.min().y),
                  std::min(box0.min().z, box1.min().z)};
    Point3D big{std::min(box0.max().x, box1.max().x),
                  std::min(box0.max().y, box1.max().y),
                  std::min(box0.max().z, box1.max().z)};
    
    Vector3D tiny_vector{Constants::Epsilon, Constants::Epsilon, Constants::Epsilon};
    
    return Bounds(small - tiny_vector, big + tiny_vector);
}