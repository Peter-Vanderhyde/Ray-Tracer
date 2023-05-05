// Graphics Sep 28, 2022
// Peter Vanderhyde
// Ray Tracing Homework

#include "ray.h"
#include "sphere.h"
#include "color.h"
#include "pixels.h"
#include "hit.h"
#include <iomanip>
#include <vector>

Color ray_color(const Ray& ray, const Sphere& sphere) {
    auto time = sphere.intersect(ray);
    if (time.has_value()) {
        Hit hit = sphere.construct_hit(ray, time.value());
        return Red;
    }
    return SkyBlue;
}

void render(const Sphere& sphere, int pixel_size) {
    const int WIDTH{1920}, HEIGHT{1080};
    Pixels screen{WIDTH, HEIGHT};
    Ray color_checker{{0, 0, 0}, {0, 1, 0}};

    for (int z = 0; z < HEIGHT / pixel_size; z++) {
        for (int x = 0; x < WIDTH / pixel_size; x++) {
            color_checker.origin = {pixel_size * x, 0, pixel_size * z};
            Color color = ray_color(color_checker, sphere);
            for (int row = 0; row < pixel_size; row++) {
                for (int col = 0; col < pixel_size; col++) {
                    screen(pixel_size * z + row, pixel_size * x + col) = color;
                }
            }
        }
    }
    screen.save_png("Rendered Scene.png");
}

int main() {
    Sphere sphere{{700, 100, 300}, 200};

    int pixel_size;
    std::cout << "Enter the pixel size ->";
    std::cin >> pixel_size;

    render(sphere, pixel_size);
}