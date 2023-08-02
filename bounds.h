#include "point3d.h"
#include "ray.h"


class Bounds {
public:
    Bounds() {}
    Bounds(const Point3D& a, const Point3D& b);

    Point3D min() const;
    Point3D max() const;

    bool intersect(const Ray& r) const;

    Point3D minimum, maximum;
};

Bounds surrounding_box(Bounds box0, Bounds box1);