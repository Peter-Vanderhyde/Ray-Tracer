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
#include <thread>
#include <iostream>
#include <vector>

// valgrind --tool=callgrind
// /Zc:tlsGuards-

void print_progress(long long ray_num, long long total_rays);
Color trace_path(const World& world, const Ray& ray, int depth);
void render(Pixels &pixels, int rows, int columns, int samples, Camera camera, World world, int depth);

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
        int thread_count = parser.threads;

        std::vector<std::thread> threads;
        std::vector<Pixels> pixel_results;

        for (int i = 0; i < thread_count; ++ i) {
            pixel_results.push_back(Pixels(pixels.columns, pixels.rows));
        }

        int num_rays = static_cast<double>(samples) / thread_count;

        for (int i = 1; i < thread_count; ++i)
        {
            std::thread t{render, std::ref(pixel_results.at(i)), pixels.rows, pixels.columns,
                            static_cast<int>(num_rays), camera, world, depth};
            threads.push_back(std::move(t));
        }
        
        render(pixel_results.at(0), pixels.rows, pixels.columns,
               static_cast<int>(num_rays), camera, world, depth);
        
        for (auto& thread : threads) {
            thread.join();
        }

        std::cout << "Combining results...\n";
        for (auto& p : pixel_results) {
            for (int y = 0; y < pixels.rows; ++y) {
                for (int x = 0; x < pixels.columns; ++x) {
                    pixels(y, x) += p(y, x);
                }
            }
        }

        for (int y = 0; y < pixels.rows; ++y) {
            for (int x = 0; x < pixels.columns; ++x) {
                pixels(y, x) /= thread_count;
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

void render(Pixels& result, int rows, int columns, int samples,
            Camera camera, World world, int depth) {
    const long long rays_total = rows * columns * static_cast<long long>(samples);
    long long ray_num = 0;
    std::thread::id main_id{1};
    if (std::this_thread::get_id() == main_id)
    {
        print_progress(ray_num, rays_total);
    }

    for (auto i = 0; i < rows; ++i) {
        for (auto j = 0; j < columns; ++j) {
            Color color{0, 0, 0};
            for (int s = 0; s < samples; ++s) {
                double x = (j + random_double()) / (columns - 1);
                double y = (i + random_double()) / (rows - 1);
                Ray ray = camera.compute_ray(x, y);
                color += trace_path(world, ray, depth);
                ++ray_num;
                if (ray_num % (rays_total / 100) == 0 && std::this_thread::get_id() == main_id) {
                    print_progress(ray_num, rays_total);
                }
            }
            result(i, j) = color / samples;
        }
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
        // This is from the ray tracing in weekend book.
        // It gives a nice blue sky gradient.
        // Vector3D unit_direction = unit(ray.direction);
        // auto t = 0.5*(unit_direction.y + 1.0);
        // return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
    }

    Material* material = hit->material;
    if (material->emitting) {
        return material->color;
    }
    Ray scattered = material->scatter(ray, hit.value());
    return trace_path(world, scattered, depth-1) * material->color;
}