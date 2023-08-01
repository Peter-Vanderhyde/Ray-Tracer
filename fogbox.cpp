#include "fogbox.h"
#include "constants.h"
#include "ray.h"
#include "shape.h"
#include "hit.h"
#include <iomanip>
#include "material.h"
#include "point2d.h"
#include "random.h"

FogBox::FogBox(const Point3D& center, const Vector3D& dimensions, const Vector3D& rotations, double density,
            std::shared_ptr<PropertyMap> property_map, Normal* normal_map)
    :Shape{property_map, normal_map}, neg_inv_density{-1/density} {
    create_box(center, dimensions, rotations, property_map, normal_map);
}

std::optional<double> FogBox::intersect(const Ray& ray) const {
    Triangle *triangle;
    int found = 0;
    double min_t = 0;
    double max_t;
    for (auto t : triangles) {
        triangle = t.get();
        std::optional<double> time = triangle->intersect(ray);
        if (time.has_value() && found == 0) {
            found += 1;
            min_t = time.value();
        }
        else if (time.has_value() && found == 1) {
            found += 1;
            max_t = time.value();
        }
    }
    if (found > 0) {
        if (found == 1) {
            max_t = min_t;
            min_t = 0;
        }
        else if (max_t < min_t) {
            std::swap(min_t, max_t);
        }

        const auto distance = max_t - min_t;
        const auto hit_distance = neg_inv_density * log(random_double());
        // std::cout << distance << " " << hit_distance << '\n';

        if (hit_distance > distance) {
            return {};
        }

        // std::cerr << "hit_distance = " << hit_distance << '\n'
        //           << "rec.t = " << min_t << '\n';

        return min_t + hit_distance;
    }
    else {
        return {};
    }
}

Hit FogBox::construct_hit(const Ray& ray, double time) const {
    // calculate surface normal
    Point3D point = ray.at(time);

    Vector3D normal = unit(-ray.direction);

    return Hit{time, point, normal, this};

}

Point2D FogBox::uv(const Hit*) const {
    return {0.0, 0.0};
}

void FogBox::create_plane(
        Point3D c1, Point3D c2, Point3D c3,
        std::shared_ptr<PropertyMap> property_map, Normal* normal_map) {
    Vector3D c4, center;
    c4 = c3 + (c1 - c2);

    triangles.push_back(std::make_shared<Triangle>(c1, c2, c3, property_map, normal_map));
    triangles.push_back(std::make_shared<Triangle>(c1, c4, c3, property_map, normal_map));
}

void FogBox::create_box(const Vector3D& center, const Vector3D& extents, const Vector3D& rotations,
                    std::shared_ptr<PropertyMap> property_map, Normal* normal_map) {
    if (extents.x <= 0.0 || extents.y <= 0.0 || extents.z <= 0.0) {
        throw std::runtime_error("Box must have length in all directions");
    }
    double half_x = extents.x / 2;
    double half_y = extents.y / 2;
    double half_z = extents.z / 2;
    std::vector<int> x_signs{-1, 1, 1, -1, 1, 1, -1, -1, 1, -1, -1, 1, -1, -1, -1, 1, 1, 1};
    std::vector<int> y_signs{-1, -1, 1, -1, -1, 1, -1, -1, -1, 1, 1, 1, -1, -1, 1, -1, -1, 1};
    std::vector<int> z_signs{-1, -1, -1, 1, 1, 1, -1, 1, 1, -1, 1, 1, -1, 1, 1, -1, 1, 1};
    Point3D c1, c2, c3;
    for (size_t i = 0; i < x_signs.size() / 3; ++i) {
        c1 = {half_x * x_signs[3 * i], half_y * y_signs[3 * i], half_z * z_signs[3 * i]};
        c2 = {half_x * x_signs[3 * i + 1], half_y * y_signs[3 * i + 1], half_z * z_signs[3 * i + 1]};
        c3 = {half_x * x_signs[3 * i + 2], half_y * y_signs[3 * i + 2], half_z * z_signs[3 * i + 2]};
        c1.rotate(rotations);
        c2.rotate(rotations);
        c3.rotate(rotations);
        c1 += center;
        c2 += center;
        c3 += center;
        create_plane(c1, c2, c3, property_map, normal_map);
    }
}