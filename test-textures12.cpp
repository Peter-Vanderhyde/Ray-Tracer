#include "camera.h"
#include "sphere.h"
#include "color.h"
#include "constants.h"
#include "pixels.h"
#include "ray.h"
#include "hit.h"
#include "texture.h"
#include "world.h"
#include "parser.h"
#include "material.h"
#include "random.h"
#include "save_details.h"
#include "point2d.h"
#include <iomanip>
#include <thread>
#include <iostream>
#include <vector>
#include "aabb.h"
#include "node.h"

// valgrind --tool=callgrind
// insight

void print_progress(long long ray_num, long long total_rays);
std::pair<Color, int> trace_path(const Node& root, const Ray& ray, int depth, bool sky);
void render(Pixels &pixels, int rows, int columns, int samples, Camera camera, Node root, int depth, bool sky);
std::optional<Hit> find_nearest(const Ray &ray, std::vector<const Shape*> shapes);

int main(int argc, char* argv[]) {
    if (argc < 1) {
        std::cout << "Usage: prog filename\n";
        return 0;
    }
    try
    {
        Parser parser("../room.txt");
        World world = parser.get_world();
        Pixels pixels = parser.get_pixels();
        Camera camera = parser.get_camera();
        int depth = parser.bounces + 1;
        int samples = parser.samples;
        int thread_count = parser.threads;
        bool sky = parser.sky;

        Node root{world.objects, 0, world.objects.size(), 0, Constants::Infinity};

        std::vector<std::thread> threads;
        std::vector<Pixels> pixel_results;

        for (int i = 0; i < thread_count; ++ i) {
            pixel_results.push_back(Pixels(pixels.columns, pixels.rows));
        }

        int num_rays = static_cast<double>(samples) / thread_count;
        std::cout << "Would check " << depth * samples * pixels.columns * pixels.rows * world.objects.size() << " times.\n";

        for (int i = 1; i < thread_count; ++i)
        {
            std::thread t{render, std::ref(pixel_results.at(i)), pixels.rows, pixels.columns,
                            static_cast<int>(num_rays), camera, root, depth, sky};
            threads.push_back(std::move(t));
        }
        
        render(pixel_results.at(0), pixels.rows, pixels.columns,
               static_cast<int>(num_rays), camera, root, depth, sky);
        
        for (auto& thread : threads) {
            thread.join();
        }

        std::cout << "\nCombining results...\n";
        for (auto& p : pixel_results) {
            for (int y = 0; y < pixels.rows; ++y)
            {
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

void render(Pixels& pixels, int rows, int columns, int samples,
            Camera camera, Node root, int depth, bool sky) {
    const long long rays_total = rows * columns * static_cast<long long>(samples);
    long long ray_num = 0;
    if (std::this_thread::get_id() == (std::thread::id)1) {
        print_progress(ray_num, rays_total);
    }

    int checked = 0;
    for (auto i = 0; i < rows; ++i)
    {
        for (auto j = 0; j < columns; ++j) {
            Color color{0, 0, 0};
            for (int s = 0; s < samples; ++s) {
                double x = (j + random_double()) / (columns - 1);
                double y = (i + random_double()) / (rows - 1);
                Ray ray = camera.compute_ray(x, y);
                std::pair<Color, int> result = trace_path(root, ray, depth, sky);
                color += result.first;
                checked += result.second;
                ++ray_num;
                if (ray_num % (rays_total / 100) == 0 && std::this_thread::get_id() == (std::thread::id)1) {
                    print_progress(ray_num, rays_total);
                }
            }
            pixels(i, j) = color / samples;
        }
    }
    std::cout << "\nActually checked " << checked << " times.\n";
}

void print_progress(long long ray_num, long long total_rays) {
    auto width = std::to_string(total_rays).length() + 4;
    //std::cout.imbue(std::locale("en-US"));
    int percentage = std::round(static_cast<double>(ray_num) / total_rays * 100);
    std::cout << "\rProgress: " << std::setw(3) << percentage << "% ";
    std::cout << std::setw(width) << ray_num << '/' << total_rays << " rays";
    std::cout << std::flush;
}

std::pair<Color, int> trace_path(const Node& root, const Ray& ray, int depth, bool sky) {
    int checked = 0;
    Color background;
    if (sky)
    {
        Vector3D unit_direction = unit(ray.direction);
        auto t = 0.5*(unit_direction.y + 1.0);
        background = (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
    }
    else {
        background = Black;
    }
    if (depth == 0) {
        return {Black, checked};
    }

    std::pair<std::optional<std::vector<const Shape*>>, int> possible_hits = root.hit(ray, 0, Constants::Infinity);
    checked = possible_hits.second;
    if (!possible_hits.first.has_value())
    {
        return {background, checked};
    }
    std::optional<Hit> hit = find_nearest(ray, possible_hits.first.value());
    checked += possible_hits.first.value().size();
    if (!hit.has_value())
    {
        return {background, checked};
    }
    Hit hit_value = hit.value();
    Point2D uv = hit_value.shape->uv(hit_value);
    
    Material* material = hit_value.shape->material;
    Texture *texture = hit_value.shape->texture;
    if (material->emitting)
    {
        Color color = texture->uv(uv.x, uv.y);
        return {color * std::pow(std::abs(dot(hit->normal, ray.direction)), 0.333), checked};
    }
    Ray scattered = material->scatter(ray, hit_value);
    std::pair<Color, int> result = trace_path(root, scattered, depth - 1, sky);
    return {result.first * texture->uv(uv.x, uv.y), checked + result.second};
}

std::optional<Hit> find_nearest(const Ray& ray, std::vector<const Shape*> shapes) {
    double time = Constants::Infinity;
    const Shape* nearest = nullptr;
    for (std::size_t i = 0; i < shapes.size(); ++i) {
        std::optional<double> t = shapes.at(i)->intersect(ray);
        if (t.has_value() && t.value() < time)
        {
            nearest = shapes.at(i);
            time = t.value();
        }
    }
    if (nearest) {
        Hit hit = nearest->construct_hit(ray, time);
        return hit;
    } else {
        return {};
    }
}