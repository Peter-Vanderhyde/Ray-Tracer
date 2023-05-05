#include "color.h"
#include "constants.h"
#include "hit.h"
#include "parser.h"
#include "pixels.h"
#include "ray.h"
#include "sphere.h"
#include "world.h"

Color trace(const World& world, const Ray& ray);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " filename\n";
        return 0;
    }
    try {
        Parser parser(argv[1]);
        World world = parser.get_world();
        Pixels pixels = parser.get_pixels();
        Camera camera = parser.get_camera();

        for (auto i = 0; i < pixels.rows; ++i) {
            std::cout << "\rProgress: " << i + 1 << "/" << pixels.rows
                      << std::flush;
            for (auto j = 0; j < pixels.columns; ++j) {
                double x = static_cast<double>(j) / (pixels.columns - 1);
                double y = static_cast<double>(i) / (pixels.rows - 1);

                Ray ray = camera.compute_ray(x, y);
                pixels(i, j) = trace(world, ray);
            }
        }
        std::string filename = parser.get_output_filename();
        pixels.save_png(filename);
        std::cout << "\nWrote " << filename << '\n';
    } catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
}

Color trace(const World& world, const Ray& ray) {
    std::optional<Hit> hit = world.find_nearest(ray);
    if (hit.has_value()) {
        return hit->normal;
    } else {
        return Black;
    }
}
