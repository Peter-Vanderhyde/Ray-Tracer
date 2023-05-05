// Peter Vanderhyde
// Graphics Project
// December 2022

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
#include <optional>

// New Features:
//  - Allows for a sun direction to provide scene-wide illumination at a specific angle
//  - Separated textures and materials so that each can be mixed and matched
//  - Every rendered image saves the contents of the parser file used to create the scene in the image
//     so it's easy to find out how the scene was made
//  - Added Vector2D and Point2D objects

// Materials:
//  - light > objects emit light
//  - diffuse > objects scatter light in a random hemisphere
//  - specular > perfectly reflects light like a mirror
//  - metal > like specular except with an extra argument of how fuzzy to make the reflection
//  - glass > uses an index ratio argument to determine the amount of light refraction
//  - gloss > creates a partially diffuse and partially metal object based on a given percentage and fuzziness
//  - foggy > creates a foggy material that just scatters light in a random direction (only for fog box shapes)
//  - point_light > makes the object with this material invisible to the camera, but casts light into the scene
//  - directional_light > (WIP) casts light in a certain angle and not beyond that angle

// Textures:
//  - solid > is one solid color
//  - gradient > creates a gradient from bluish to pinkish
//  - swirl > creates swirls with the ability to determine the colors, number of swirls, and thickness of swirls
//  - squares > creates squares of specified color with specified size
//  - checkered > creates a checkered pattern of the specified colors
//  - image > takes a png image and maps it to the uv coordinates of whatever object it's applied to

// Parse Objects:
//  - sphere > can use any texture
//  - triangle > cannot use image texture
//  - textured_triangle > must use image texture by specifying the image coords on the triangle vecs
//  - plane > creates a plane by specifying three of the corner coords. cannot use image texture
//  - textured_plane > must use image texture
//  - box > creats a box at a specified position with given dimensions and rotation. cannot use image texture
//  - textured_box > must use image texture
//  - fog_box > a box specifically for the foggy material to create fog of the given density
//  - mesh > uses a text file to create triangles. cannot use image texture
//  - obj > uses obj file to create object that can be scaled and rotated. can use the image texture, but it will not
//     look right if it's not the texture used for the object. (might not look completely right anyway)

// Other Parsing Keywords:
//  - camera > position and rotate a virtual camera with a given FOV
//  - pixels > specifies the images width and height
//  - output > the filename of the output file
//  - rays > specifies the number of bounces the rays can do and the number of rays per pixel
//  - material > defines a material with the given name
//  - texture > defines a texture with the given name
//  - threads > specifies how many threads to use. loses effectiveness past the number of computer cores
//  - sun > specifies which direction the sun is, so the scene is lit from that angle

void print_progress(long long ray_num, long long total_rays);
Color trace_path(const World& world, const Ray& ray, int max_depth,
                int curr_depth, std::optional<Vector3D> sun_direction);
void render(Pixels &pixels, int rows, int columns, int samples,
            Camera camera, World world, int depth, std::optional<Vector3D> sun_direction);
double angle(const Vector3D &v1, const Vector3D &v2);
double clamp(double value, double min, double max);

int main(int argc, char* argv[]) {
    if (argc < 1) {
        std::cout << "Usage: prog filename\n";
        return 0;
    }
    try
    {
        Parser parser("../test.txt");
        World world = parser.get_world();
        Pixels pixels = parser.get_pixels();
        Camera camera = parser.get_camera();
        int depth = parser.bounces + 1;
        int samples = parser.samples;
        int thread_count = parser.threads;
        std::optional<Vector3D> sun_direction = parser.get_sun_direction();

        std::vector<std::thread> threads;
        std::vector<Pixels> pixel_results;

        for (int i = 0; i < thread_count; ++ i) {
            pixel_results.push_back(Pixels(pixels.columns, pixels.rows));
        }

        int num_rays = static_cast<double>(samples) / thread_count;

        for (int i = 1; i < thread_count; ++i)
        {
            std::thread t{render, std::ref(pixel_results.at(i)), pixels.rows, pixels.columns,
                            static_cast<int>(num_rays), camera, world, depth, sun_direction};
            threads.push_back(std::move(t));
        }
        
        render(pixel_results.at(0), pixels.rows, pixels.columns,
               static_cast<int>(num_rays), camera, world, depth, sun_direction);
        
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
            Camera camera, World world, int depth, std::optional<Vector3D> sun_direction) {
    const long long rays_total = rows * columns * static_cast<long long>(samples);
    long long ray_num = 0;
    if (std::this_thread::get_id() == (std::thread::id)1) {
        print_progress(ray_num, rays_total);
    }

    for (auto i = 0; i < rows; ++i) {
        for (auto j = 0; j < columns; ++j) {
            Color color{0, 0, 0};
            for (int s = 0; s < samples; ++s) {
                double x = (j + random_double()) / (columns - 1);
                double y = (i + random_double()) / (rows - 1);
                Ray ray = camera.compute_ray(x, y);
                color += trace_path(world, ray, depth, 0, sun_direction);
                ++ray_num;
                if (ray_num % (rays_total / 100) == 0 && std::this_thread::get_id() == (std::thread::id)1) {
                    print_progress(ray_num, rays_total);
                }
            }
            pixels(i, j) = color / samples;
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

Color trace_path(const World& world, const Ray& ray, int max_depth, int curr_depth, std::optional<Vector3D> sun_direction) {
    if (curr_depth == max_depth) {

        return Black;
    }

    std::optional<Hit> hit = world.find_nearest(ray, curr_depth);
    if (!hit.has_value()) {
        if (sun_direction.has_value()) {
            Vector3D unit_direction = unit(-ray.direction);
            Vector3D sun_dir = unit(sun_direction.value());
            double angle_diff = angle(unit_direction, sun_dir);
            if (angle_diff <= 30.0) {
                double t = std::abs(dot(unit_direction, sun_dir));
                return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(1.0, 1.0, 1.0);
            }
            else {
                return Black;
            }
        }
        else {
            return Black;
        }
        // Can use this instead of black to create a sky color surrounding the scene
        // Vector3D unit_direction = unit(ray.direction);
        // auto t = 0.5*(unit_direction.y + 1.0);
        // return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
    }
    Hit hit_value = hit.value();
    
    Material* material = hit_value.shape->material;
    Texture *texture = hit_value.shape->texture;
    Point2D uv = hit_value.shape->uv(hit_value);
    if (material->emitting)
    {
        Color color = texture->uv(uv.x, uv.y);
        color *= std::pow(std::abs(dot(hit->normal, ray.direction)), 0.333);
        return color;
    }
    Ray scattered = material->scatter(ray, hit_value);
    return trace_path(world, scattered, max_depth, curr_depth+1, sun_direction) * texture->uv(uv.x, uv.y);
}

double angle(const Vector3D& v1, const Vector3D& v2) {
    return acos(clamp(dot(v1, v2), -1.0, 1.0)) * (180.0 / Constants::Pi);
}

double clamp(double value, double min, double max) {
    return std::max(min, std::min(value, max));
}