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
#include "save_details.h"
#include <iomanip>

// powershell "Measure-Command {./main.exe ../input}"

void print_progress(long long ray_num, long long total_rays);
Color trace_path(const World& world, const Ray& ray, int depth);
double material_index_at_ray_origin(const World& world, const Ray& ray);

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
                    ray.index_ratio = material_index_at_ray_origin(world, ray);
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
        save_details(filename, argv[1], pixels.columns * pixels.rows * 4);
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

    Material* material = hit->material;
    if (material->emitting) {
        return material->color;
    }
    Ray scattered = material->scatter(ray, hit.value());
    return trace_path(world, scattered, depth-1) * material->color;
}

double material_index_at_ray_origin(const World& world, const Ray& ray) {
    std::vector<std::shared_ptr<Sphere>> inside;
    for (auto object : world.objects)
    {
        if (length(ray.origin - object->center) < object->radius - Constants::Epsilon) {
            inside.push_back(object);
        }
    }

    if (inside.size() == 0) {
        return 1.0;
    }
    else {
        double closest{Constants::Infinity};
        std::shared_ptr<Sphere> closest_object{nullptr};
        for (auto object : inside)
        {
            std::optional<double> t = object->intersect(Ray(ray.origin, ray.direction * -1));
            if (t.has_value() && t.value() < closest) {
                closest = t.value();
                closest_object = object;
            }
        }
        return closest_object->material->index_ratio;
    }
}