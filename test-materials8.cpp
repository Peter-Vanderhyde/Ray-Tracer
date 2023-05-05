#include "camera.h"
#include "sphere.h"
#include "color.h"
#include "constants.h"
#include "pixels.h"
#include "ray.h"
#include "hit.h"
#include "world.h"
#include "parser.h"
#include "material.h"
#include "random.h"
// #include "save_details.h" <- my file that writes input in the image
#include <iomanip>

void print_progress(long long ray_num, long long total_rays);
Color trace_path(const World& world, const Ray& ray, int depth);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: prog filename\n";
        return 0;
    }
    try
    {
        Parser parser(argv[1]);
        World world = parser.get_world();
        Pixels pixels = parser.get_pixels();
        Camera camera = parser.get_camera();
        int depth = parser.depth;
        int samples = parser.samples;

        const long long rays_total = pixels.rows * pixels.columns * static_cast<long long>(samples);
        long long ray_num = 0;
        print_progress(ray_num, rays_total);
        for (auto i = 0; i < pixels.rows; ++i) {
            for (auto j = 0; j < pixels.columns; ++j) {
                Color color{0, 0, 0};
                for (int s = 0; s < samples; ++s) {
                    double x = (j + random_double()) / (pixels.columns - 1);
                    double y = (i + random_double()) / (pixels.rows - 1);
                    Ray ray = camera.compute_ray(x, y);
                    color += trace_path(world, ray, depth);
                    ++ray_num;
                    if (ray_num % (rays_total / 100) == 0) {
                        print_progress(ray_num, rays_total);
                    }
                }
                pixels(i, j) = color / samples;
            }
        }
        std::string filename = parser.get_output_filename();
        pixels.save_png(filename);
        std::cout << "\nWrote " << filename << '\n';
        // save_details(filename, argv[1], pixels.columns * pixels.rows * 4);
    }
    catch (std::exception& err) {
        std::cout << err.what() << '\n';
    }
}

void print_progress(long long ray_num, long long total_rays) {
    auto width = std::to_string(total_rays).length() + 4;
    //std::cout.imbue(std::locale("en-US"));
    int percentage = std::round(static_cast<double>(ray_num) / total_rays * 100);
    std::cout << "\rProgress: " << std::setw(3) << percentage << "% ";
    std::cout << std::setw(width) << ray_num << '/' << total_rays << " rays";
    std::cout << std::flush;
}

Color trace_path(const World& world, const Ray& ray, int depth) {
    if (depth == 0) {
        return Black;
    }

    std::optional<Hit> hit = world.find_nearest(ray);
    if (!hit.has_value()) {
        return Black;
    }

    std::shared_ptr<Material> material = hit->material;
    if (material->emitting) {
        return material->color;
    }
    Ray scattered = material->scatter(ray, hit.value());
    return trace_path(world, scattered, depth-1) * material->color;
}
