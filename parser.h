#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <memory>

#include "camera.h"
#include "pixels.h"
#include "world.h"

class Material;
class Texture;

using Materials = std::map<std::string, std::shared_ptr<Material>>;
using Textures = std::map<std::string, std::shared_ptr<Texture>>;

class Parser {
public:
    Parser(const std::string& filename);
    Camera get_camera();
    World get_world();
    Pixels get_pixels();
    std::string get_output_filename();
    std::optional<Vector3D> get_sun_direction();

    int bounces, samples;
    int threads{1};

private:
    std::string input_filename;
    bool found_camera, found_pixels, found_output, found_rays;
    bool found_sun = false;
    Point3D camera_position, camera_target;
    Vector3D camera_up, sun_direction;
    double camera_fov, aspect;
    int columns, rows;
    World world;
    std::string output_filename;
    Materials materials;
    Textures textures;

    void parse(std::ifstream& input);
    void verify();
    void parse_camera(std::stringstream& ss);
    void parse_pixels(std::stringstream& ss);
    void parse_sphere(std::stringstream& ss);
    void parse_triangle(std::stringstream& ss);
    void parse_textured_triangle(std::stringstream& ss);
    void parse_textured_triangle(Vector3D v0, Vector3D v1, Vector3D v2,
                                   Point2D t0, Point2D t1, Point2D t2,
                                   std::string material_name, std::string texture_name);
    void parse_plane(std::stringstream &ss);
    void parse_plane(Point3D c1, Point3D c2, Point3D c3,
                     std::string material_name, std::string texture_name);
    void parse_textured_plane(std::stringstream &ss);
    void parse_textured_plane(Vector3D c1, Vector3D c2, Vector3D c3,
                                Point2D tile, std::string material_name, std::string texture_name);
    void parse_box(std::stringstream &ss);
    void parse_textured_box(std::stringstream &ss);
    void parse_fog_box(std::stringstream &ss);
    void parse_fog_sphere(std::stringstream &ss);
    void parse_mesh(std::stringstream &ss);
    void parse_obj(std::stringstream& ss);
    void parse_output(std::stringstream &ss);
    void parse_rays(std::stringstream& ss);
    void parse_material(std::stringstream &ss);
    void parse_texture(std::stringstream &ss);
    void parse_threads(std::stringstream &ss);
    void parse_sun(std::stringstream &ss);
    Material *get_material(std::string name);
    Texture* get_texture(std::string name);
};

void remove_comment(std::string& line);
bool is_whitespace(const std::string& line);