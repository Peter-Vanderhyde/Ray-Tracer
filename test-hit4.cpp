#include "ray.h"
#include "constants.h"
#include "sphere.h"
#include "color.h"
#include "pixels.h"
#include "hit.h"
#include <iomanip>
#include <vector>
#include <utility>

using Environment = std::vector<Sphere>;

Color ray_color(const Ray& ray, const Environment& env) {
    double closest_time = -1.0;
    Sphere closest_sphere{{0, 0, 0}, 1};
    for (auto sphere : env) {
        auto time = sphere.intersect(ray);
        if (time.has_value() && (closest_time < 0 || time < closest_time)) {
            closest_time = time.value();
            closest_sphere = sphere;
        }
    }
    if (closest_time < 0) {
        return Black;
    }
    Hit hit = closest_sphere.construct_hit(ray, closest_time);
    // double shading = dot(hit.normal, Vector3D{1, -1, -1});
    // return hit.normal * shading;
    return hit.normal;
}

void render(const Environment& env, char side) {
    const int WIDTH{1920}, HEIGHT{1080};
    Pixels screen{WIDTH, HEIGHT};
    if (side == 'f') {
        Ray color_checker{{0, -500, 0}, {0, 1, 0}};

        for (int z = 0; z > -HEIGHT; z--) {
            for (int x = 0; x < WIDTH; x++) {
                color_checker.origin = {-WIDTH / 2 + x, -500, HEIGHT / 2 + z};
                Color color = ray_color(color_checker, env);
                screen(-1 * z, x) = color;
            }
        }
    }
    else if (side == 's') {
        Ray color_checker{{900, 0, 0}, {-1, 0, 0}};

        for (int z = 0; z > -HEIGHT; z--) {
            for (int y = 0; y < WIDTH; y++) {
                color_checker.origin = {900, -WIDTH / 2 + y, HEIGHT / 2 + z};
                Color color = ray_color(color_checker, env);
                screen(-1 * z, y) = color;
            }
        }
    }
    else if (side == 't') {
        Ray color_checker{{0, HEIGHT / 2, 500}, {0, 0, -1}};

        for (int y = 0; y > -HEIGHT; y--) {
            for (int x = 0; x < WIDTH; x++) {
                color_checker.origin = {-WIDTH / 2 + x, HEIGHT + y, 500};
                Color color = ray_color(color_checker, env);
                screen(-1 * y, x) = color;
            }
        }
    }
    screen.save_png("Rendered Scene.png");
}

int main() {
    Environment env;
    env.push_back(Sphere{{0, 425, 0}, 400});
    env.push_back(Sphere{{400, 425, 0}, 300});
    env.push_back(Sphere{{325, 201, 50}, 200});

    char side;
    std::cout << "Render from front, side, or top? (f/s/t) ->";
    std::cin >> side;

    render(env, side);
}