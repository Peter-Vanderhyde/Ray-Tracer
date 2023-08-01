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
#include "sun.h"
#include "combine_images.h"
#include "skysphere.h"
#include <iomanip>
#include <thread>
#include <iostream>
#include <vector>
#include <optional>
#include <chrono>
#include <filesystem>
#include <csignal>

// New Features:
//  - Allows for a sun direction to provide scene-wide illumination at a specific angle
//    (Use the vertex (3.14 1.59 2.65) to get a random sun angle)
//  - Separated textures and materials so that each can be mixed and matched
//  - Every rendered image saves the contents of the parser file used to create the scene in the image
//     so it's easy to find out how the scene was made
//  - Added Vector2D and Point2D objects
//  - Implemented normal maps so they can be used
//  - Can now create depth of field

// Materials:
//  - light > objects emit light
//  - diffuse > objects scatter light in a random hemisphere
//  - specular > has a range of properties from a mirror to totally diffuse with glossy or metalic in between
//  - metal > like specular except with an extra argument of how fuzzy to make the reflection
//  - glass > uses an index ratio argument to determine the amount of light refraction
//  - gloss > creates a partially diffuse and partially specular object
//  - foggy > creates a foggy material that just scatters light in a random direction
//  - point_light > makes the object with this material invisible to the camera, but casts light into the scene
//  - directional_light > (wip) casts light in a certain angle and not beyond that angle
//  - fuzzy_gloss > mixes diffuse and metal. It takes a diffuse percent and metal fuzz amount

// Textures:
//  - solid > is one solid color
//  - gradient > creates a gradient from bluish to pinkish
//  - swirl > creates swirls with the ability to determine the colors, number of swirls, and thickness of swirls
//  - squares > creates squares of specified color with specified size
//  - checkered > creates a checkered pattern of the specified colors
//  - image > takes a png image and maps it to the uv coordinates of whatever object it's applied to
//  - specular_texture > a texture used in conjunction with a specular material to create a specific look

// Normals:
//  - normal name filename (1 1 1) > creates a normal that can be used with the normal_sphere,
//     normal_triangle, normal_plane or normal_box objects. The last argument is whether to invert that
//     normal axis or not

// Objects:
//  - sphere > can use any texture
//  - normal_sphere > sphere but with normal map
//  - triangle > cannot use image texture
//  - textured_triangle > must use image texture by specifying the image coords on the triangle vecs
//  - normal_triangle > uses a normal map and the image coords to create a normal texture
//  - billboard_triangle > can only be seen from one side
//  - plane > creates a plane by specifying three of the corner coords. cannot use image texture
//  - textured_plane > must use image texture
//  - normal_plane > uses the tile arg to tile the normal map
//  - billboard_plane > a plane that can only be seen from one side (Order the corners topleft, botleft, botright)
//  - box > creates a box at a specified position with given dimensions and rotation. cannot use image texture
//  - textured_box > must use image texture
//  - normal_box > adds normal on each side (normals may not be correct on all sides yet)
//  - fog_box > a box specifically for the foggy material to create fog of the given density
//  - mesh > uses a text file to create triangles. cannot use image texture or normal
//  - obj > uses obj file to create object that can be scaled and rotated. can use the image texture, but it will not
//     look right if it's not the texture used for the object. (seems to not be applied correctly anyway)

// Other Parsing Keywords:
//  - camera > position and rotate a virtual camera with a given FOV
//  - lens > Specifies the percentage toward the camera looking at vector and blur amount
//  - pixels > specifies the images width and height
//  - output > the filename of the output file
//  - rays > specifies the number of bounces the rays can do and the number of rays per pixel
//  - material > defines a material with the given name
//  - texture > defines a texture with the given name
//  - threads > specifies how many threads to use. loses effectiveness past the number of computer cores
//  - sun > specifies which direction the sun is, so the scene is lit from that angle
//  - sky > makes the background a blue sky and gray ground color instead of black. This will add blue color to the scene
//  - checkpoints > tells how many times to create a new checkpoint image. Useful during long renders to see render progress


/*
Add specular map support
Change specular to allow editing both specular color and gloss amount
Bloom maybe
multi-material objects
Add sphere rotations so textures can be rotated
Make obj textures line up correctly (normal map too?)
Collision tree
Fix saving details
Look into smoothing obj by interpolating normals between edges
Fix normal on box
Fix metal freaking out
*/

namespace fs = std::filesystem;

void print_progress(long long ray_num, long long total_rays, std::chrono::seconds elapsed_time, int rays_done);
Color trace_path(const World& world, const Ray& ray, int max_depth, int curr_depth,
                std::optional<Sun> opt_sun, bool sky, std::optional<Skysphere> skysphere);
void render(Pixels &pixels, int rows, int columns, int samples, Camera camera,
            World world, int depth, std::optional<Sun> sun, bool sky, std::optional<Skysphere> skysphere);
double angle(const Vector3D &v1, const Vector3D &v2);
double clamp(double value, double min, double max);
Color get_background_color(const Ray& ray, std::optional<Sun> sun, bool sky, std::optional<Skysphere> skysphere);
Color get_sky_color(const Ray& ray, bool sky, std::optional<Skysphere> skysphere);
Color get_sunlight(Sun sun, const Ray& ray, Color sky_color);
void delete_png_images(const std::string& directoryPath);
void signal_handler(int signal);
Color mix(double mixFactor, const Color& color1, const Color& color2);

int main(int argc, char* argv[]) {
    if (argc < 1) {
        std::cout << "Usage: prog filename\n";
        return 0;
    }
    try
    {
        std::cout << "Parsing text file...\n";
        const std::string scene_filename{argv[1]};
        Parser parser("files/scene_files/" + scene_filename);
        World world = parser.get_world();
        Pixels pixels = parser.get_pixels();
        Camera camera = parser.get_camera();
        int depth = parser.bounces + 1;
        int samples = parser.samples;
        int thread_count = parser.threads;
        std::optional<Sun> sun = parser.sun;
        std::optional<Skysphere> skysphere = parser.skysphere;
        bool sky = parser.has_sky();
        int checkpoints = parser.get_checkpoints();

        int samples_per_checkpoint;
        if (checkpoints == 0) {
            samples_per_checkpoint = samples;
            checkpoints = 1;
        } else {
            samples_per_checkpoint = static_cast<int>(round(samples / checkpoints));
        }

        if (samples_per_checkpoint < thread_count) {
            throw std::runtime_error(
                "More threads than rays. " + std::to_string(samples_per_checkpoint) + " rays per checkpoint."
            );
        }

        std::cout << "RAYS PER CHECKPOINT: " << samples_per_checkpoint << "\n\n";
        delete_png_images("files/checkpoints");
        // Handles Ctrl-C interrupt
        std::signal(SIGINT, signal_handler);

        for (int checkpoint = 0; checkpoint < checkpoints; ++checkpoint) {
            std::vector<std::thread> threads;
            std::vector<Pixels> pixel_results;

            for (int i = 0; i < thread_count; ++ i) {
                pixel_results.push_back(Pixels(pixels.columns, pixels.rows));
            }

            int num_rays = static_cast<double>(samples_per_checkpoint) / thread_count;

            for (int i = 1; i < thread_count; ++i)
            {
                std::thread t{render, std::ref(pixel_results.at(i)), pixels.rows, pixels.columns,
                                static_cast<int>(num_rays), camera, world, depth, sun, sky, skysphere};
                threads.push_back(std::move(t));
            }
            
            render(pixel_results.at(0), pixels.rows, pixels.columns,
                static_cast<int>(num_rays), camera, world, depth, sun, sky, skysphere);
            
            for (auto& thread : threads) {
                thread.join();
            }

            std::cout << "\nCombining threaded results...\n";
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

            std::string output_filename = parser.get_output_filename();
            pixels.save_png("files/checkpoints/" + std::to_string(checkpoint) + output_filename);
            std::cout << "Saved checkpoint " << checkpoint + 1 << " of " << checkpoints << ".\n";
            save_details("files/checkpoints/" + std::to_string(checkpoint) + output_filename,
                            "files/scene_files/" + scene_filename, pixels.columns * pixels.rows * 4);
            output_filename = parser.get_output_filename();
            createAverageImage("files/checkpoints", "files/renders/" + output_filename);
            std::cout << "\nWrote " << output_filename << '\n';
            save_details("files/renders/" + output_filename,
                            "files/scene_files/" + scene_filename, pixels.columns * pixels.rows * 4);
        }
    }
    catch (std::exception& err) {
        std::cout << err.what() << '\n';
    }
}

void render(Pixels& pixels, int rows, int columns, int samples,
            Camera camera, World world, int depth, std::optional<Sun> sun, bool sky,
            std::optional<Skysphere> skysphere) {
    const long long rays_total = rows * columns * static_cast<long long>(samples);
    long long ray_num = 0;
    if (std::this_thread::get_id() == (std::thread::id)1) {
        std::chrono::minutes first_time{0};
        print_progress(ray_num, rays_total, first_time, 0);
    }

    auto last_time = std::chrono::high_resolution_clock::now();
    int last_rays_traced = 0;

    for (auto i = 0; i < rows; ++i) {
        for (auto j = 0; j < columns; ++j) {
            Color color{0, 0, 0};
            for (int s = 0; s < samples; ++s) {
                double x = (j + random_double()) / (columns - 1);
                double y = (i + random_double()) / (rows - 1);
                Ray ray = camera.compute_ray(x, y);
                color += trace_path(world, ray, depth, 0, sun, sky, skysphere);
                ++ray_num;
                if (ray_num % (rays_total / 1000) == 0 && std::this_thread::get_id() == (std::thread::id)1) {
                    auto current_time = std::chrono::high_resolution_clock::now();
                    auto elapsed{std::chrono::duration_cast<std::chrono::seconds>(current_time - last_time)};
                    if (elapsed.count() < 1 && ray_num != rays_total) {
                        print_progress(ray_num, rays_total, elapsed, 0);
                    }
                    else {
                        print_progress(ray_num, rays_total, elapsed, ray_num - last_rays_traced);
                        last_time = current_time;
                        last_rays_traced = ray_num;
                    }
                }
            }
            pixels(i, j) = color / samples;
        }
    }
}

void print_progress(long long ray_num, long long total_rays, std::chrono::seconds elapsed_time, int rays_done) {
    auto width = std::to_string(total_rays).length() + 4;
    //std::cout.imbue(std::locale("en-US"));
    int percentage = static_cast<double>(ray_num) / total_rays * 100;
    std::cout << "\rProgress: " << std::setw(3) << percentage << "% ";
    std::cout << std::setw(width) << ray_num << '/' << total_rays << " rays";
    if (rays_done != 0) {
        std::string remaining;
        std::chrono::seconds seconds_left = elapsed_time;
        seconds_left *= ((total_rays - ray_num) / rays_done);
        if (seconds_left.count() >= 60)
        {
            auto minutes_left = std::chrono::duration_cast<std::chrono::minutes>(seconds_left);
            remaining = std::to_string(minutes_left.count()) + " minutes";
            int extra_seconds = seconds_left.count() % 60;
            remaining += " " + std::to_string(extra_seconds) + " seconds";
        }
        else
        {
            remaining = std::to_string(seconds_left.count()) + " seconds";
        }
        std::string max_string = "100 minutes 59 seconds";
        std::cout << "\tApprox time remaining: " << std::setw(max_string.length()) << remaining;
    }
    std::cout << std::flush;
}

Color trace_path(const World& world, const Ray& ray, int max_depth, int curr_depth,
                std::optional<Sun> sun, bool sky, std::optional<Skysphere> skysphere) {
    if (curr_depth == max_depth) {

        return get_background_color(ray, sun, sky, skysphere);
    }

    std::optional<Hit> hit = world.find_nearest(ray, curr_depth);
    if (!hit.has_value()) {
        return get_background_color(ray, sun, sky, skysphere);
    }
    Hit hit_value = hit.value();
    Point2D hit_uv = hit_value.uv;

    Material *material = hit_value.shape->get_material(hit_uv.x, hit_uv.y);
    Texture *texture = hit_value.shape->get_texture(hit_uv.x, hit_uv.y);
    double opacity = texture->opacity(hit_uv.x, hit_uv.y);
    std::optional<Color> passthrough = {};
    if (opacity != 1.0) {
        passthrough = trace_path(world, Ray(hit_value.position, ray.direction), max_depth, curr_depth, sun, sky, skysphere);
        if (opacity == 0.0) {
            return passthrough.value();
        }
    }

    // Apply any normal maps to the shape normal
    Vector3D shape_normal = hit_value.shape->normal_map->get_vector(hit_uv, hit_value.normal);
    if (length(shape_normal) == 0) {
        // Something went wrong with the normal calculation
        return Red;
    }

    // Apply normal map adjustments to the hit normal
    hit_value.normal = shape_normal;

    if (material->emitting)
    {
        Color color = texture->uv(hit_uv.x, hit_uv.y);
        color *= std::pow(std::abs(dot(hit_value.normal, ray.direction)), 0.333);
        if (opacity != 1.0) {
            // Combine transparent color with color behind it
            return opacity * color + (1.0 - opacity) * passthrough.value();
        }
        return color;
    }

    Ray scattered = material->scatter(ray, hit_value);
    Color texture_color = texture->uv(hit_uv.x, hit_uv.y);
    Color color = trace_path(world, scattered, max_depth, curr_depth+1, sun, sky, skysphere) * texture_color;
    if (opacity != 1.0) {
        // combine transparent color with color behind it
        return opacity * color + (1.0 - opacity) * passthrough.value();
    }
    return color;
}

double angle(const Vector3D& v1, const Vector3D& v2) {
    return acos(dot(v1, v2) / fabs(length(v1) * length(v2))) * (180.0 / Constants::Pi);
}

double clamp(double value, double min, double max) {
    return std::max(min, std::min(value, max));
}

Color get_background_color(const Ray& ray, std::optional<Sun> sun, bool sky, std::optional<Skysphere> skysphere) {
    Color sky_color = get_sky_color(ray, sky, skysphere);
    if (sun.has_value()) {
        return get_sunlight(sun.value(), ray, sky_color);
    }
    
    return sky_color;
}

Color get_sky_color(const Ray& ray, bool sky, std::optional<Skysphere> skysphere) {
    if (skysphere.has_value()) {
        std::optional<double> time = skysphere.value().intersect(ray);
        if (time.has_value()) {
            Point2D uv = skysphere.value().uv(ray, time.value());
            return skysphere.value().texture->uv(uv.x, uv.y);
        }
    }
    if (sky) {
        Vector3D unit_direction = unit(ray.direction);
        if (unit_direction.z > 0) {
            auto white_amount = pow(1 - unit_direction.z, 5.0);
            return white_amount * Color(1.0, 1.0, 1.0) + (1.0 - white_amount) * Color(0.035, 0.404, 0.824);
        } else {
            return Color(0.416, 0.392, 0.380);
        }
    }
    else {
        return Black;
    }
}

Color get_sunlight(Sun sun, const Ray& ray, Color sky_color) {
    Vector3D unit_direction = unit(-ray.direction);
    double angle_diff = angle(unit_direction, sun.direction);
    if (angle_diff <= sun.size) {
        double t = angle_diff / sun.size;
        return t * sky_color + (1.0 - t) * sun.color * sun.intensity;
    }
    else {
        return sky_color;
    }
}

void delete_png_images(const std::string& directoryPath) {
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".png") {
            if (!(fs::remove(entry.path()))) {
                throw std::runtime_error("Failed to delete image.");
            }
        }
    }
}

void signal_handler(int signal) {
    if (signal == SIGINT) {
        std::string output_filename = "cancelled_render.png";
        createAverageImage("files/checkpoints", "files/renders/" + output_filename);
        std::cout << "\nWrote " << output_filename << '\n';

        exit(signal);
    }
}

Color mix(double mixFactor, const Color& color1, const Color& color2) {
    // Clamp the mixFactor to the range [0, 1]
    mixFactor = std::clamp(mixFactor, 0.0, 1.0);

    // Linear interpolation between color1 and color2 based on mixFactor
    return (1.0 - mixFactor) * color1 + mixFactor * color2;
}