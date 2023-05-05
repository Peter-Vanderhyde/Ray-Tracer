#include "camera.h"
#include "sphere.h"
#include "vector3d.h"
#include "constants.h"
#include "color.h"
#include "pixels.h"
#include "hit.h"
#include "ray.h"
#include "constants.h"
#include <optional>

double degree_to_radian(double degrees) {
    return degrees * Constants::Pi / 180;
}

void create_image(Point3D position, Vector3D up, double fov, std::string filename) {
    Sphere ground{{0, 0, -100}, 100};
    Sphere sphere{{0, 0, 1}, 1};

    Pixels pixels{1280, 720};
    Vector3D target{sphere.center};
    double aspect = static_cast<double>(pixels.columns) / pixels.rows;

    Camera camera{position, target, up, degree_to_radian(fov), aspect};


    for (int row = 0; row < pixels.rows; ++row) {
        // inclusive y:[0, 1.0]
        double y = static_cast<double>(row) / (pixels.rows - 1);
        for (int col = 0; col < pixels.columns; ++col) {
            double x = static_cast<double>(col) / (pixels.columns - 1);
            Ray ray = camera.compute_ray(x, y);

            std::optional<double> time = ground.intersect(ray);
            if (time.has_value()) {
                Hit hit = ground.construct_hit(ray, time.value());
                double shading = std::abs(dot(ray.direction, hit.normal));
                pixels(row, col) = White * shading;
            }
            time = sphere.intersect(ray);
            if (time.has_value()) {
                Hit hit = sphere.construct_hit(ray, time.value());
                pixels(row, col) = hit.normal;
            }
        }
    }

    pixels.save_png(filename);
    std::cout << "Wrote " << filename << '\n';
}

int main() {
    Vector3D position, up;
    double fov;

    // First image from the default position with fov of 22
    position = {0, -10, 10};
    up = {0, 0, 1};
    fov = 22;
    // create_image(position, up, fov, filename)
    create_image(position, up, fov, "rotate1.png");

    // Second image looking towards negative x from 50 on the x and 30 on the z
    position = {50, 0, 30};
    create_image(position, up, fov, "rotate2.png");
    
    // Third image looking towards the negative y rotated by 45 degrees
    position = {0, 50, 0};
    up = {-1, 0, 1};
    create_image(position, up, fov, "rotate3.png");

    // Fourth image changing the FOV from the same perspective as the last image
    // FOV now 70
    fov = 70;
    create_image(position, up, fov, "fov4.png");
    
    // Fifth image from the default position (0 -10 10)
    // FOV now 130
    position = {0, -10, 10};
    up = {0, 0, 1};
    fov = 130;
    create_image(position, up, fov, "fov5.png");
}