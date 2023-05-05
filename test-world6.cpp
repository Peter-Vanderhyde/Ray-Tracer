#include "camera.h"
#include "sphere.h"
#include "vector3d.h"
#include "constants.h"
#include "color.h"
#include "pixels.h"
#include "hit.h"
#include "ray.h"
#include "constants.h"
#include "world.h"
#include <optional>

double degree_to_radian(double degrees) {
    return degrees * Constants::Pi / 180;
}

Color trace(const World &world, const Ray &ray);

int main() {
    World world;
    world.add({0, 0, -100}, 100);
    world.add({0, 0, 1}, 1);
    world.add({0, 0, 1}, 1);

    Pixels pixels{1080, 720};

    Vector3D position{0, -10, 5}, up{0, 0, 1};
    Vector3D target{0, 0, 0};
    double fov{70};
    double aspect = static_cast<double>(pixels.columns) / pixels.rows;
    Camera camera{position, target, up, degree_to_radian(fov), aspect};

    for (int row = 0; row < pixels.rows; ++row) {
        // inclusive y:[0, 1.0]
        double y = static_cast<double>(row) / (pixels.rows - 1);
        for (int col = 0; col < pixels.columns; ++col) {
            double x = static_cast<double>(col) / (pixels.columns - 1);
            Ray ray = camera.compute_ray(x, y);

            pixels(row, col) = trace(world, ray);
        }
    }

    std::string filename{"sphere.png"};
    pixels.save_png(filename);
    std::cout << "Wrote " << filename;
}

Color trace(const World& world, const Ray& ray) {
    std::optional<Hit> hit = world.find_nearest(ray);
    if (hit.has_value()) {
        return hit->normal;
    }
    else {
        return Black;
    }
}