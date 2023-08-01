#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <memory>

#include "camera.h"
#include "pixels.h"
#include "world.h"
#include "color.h"
#include "sun.h"
#include "skysphere.h"

class Material;
class Texture;
class Normal;
class SpecularMap;

using Materials = std::map<std::string, std::shared_ptr<Material>>;
using Textures = std::map<std::string, std::shared_ptr<Texture>>;
using Normals = std::map<std::string, std::shared_ptr<Normal>>;
using Speculars = std::map<std::string, std::shared_ptr<SpecularMap>>;

class Parser {
public:
    Parser(const std::string& filename);
    Camera get_camera();
    World get_world();
    Pixels get_pixels();
    std::string get_output_filename();
    bool has_sky();
    int get_checkpoints();

    int bounces, samples;
    int threads{1};
    std::optional<Sun> sun = {};
    std::optional<Skysphere> skysphere = {};

private:
    std::string input_filename;
    bool found_camera, found_pixels, found_output, found_rays;
    bool found_sky = false;
    Point3D camera_position, camera_target;
    Vector3D camera_up;
    double camera_fov, aspect;
    int columns, rows;
    World world;
    std::string output_filename;
    int checkpoints = 0;
    double focus_dist = 1, blur_strength = 0;

    Materials materials;
    Textures textures;
    Normals normals{{"generic", std::make_shared<FlatNormal>(Vector3D{1, 1, 1})}};
    Speculars speculars;

    void parse(std::ifstream& input);
    void verify();
    void parse_camera(std::stringstream& ss);
    void parse_lens(std::stringstream& ss);
    void parse_pixels(std::stringstream& ss);
    void parse_sphere(std::stringstream& ss);
    void parse_specular_sphere(std::stringstream& ss);
    void parse_normal_sphere(std::stringstream& ss);
    void parse_normal_specular_sphere(std::stringstream& ss);
    void parse_triangle(std::stringstream& ss);
    void parse_textured_triangle(Vector3D v0, Vector3D v1, Vector3D v2,
                                   Point2D t0, Point2D t1, Point2D t2,
                                   std::string material_name, std::string texture_name, std::string normal_name);
    void parse_textured_triangle(std::stringstream& ss);
    void parse_normal_triangle(std::stringstream& ss);
    void parse_billboard_triangle(Vector3D v0, Vector3D v1, Vector3D v2,
                                   std::string material_name, std::string texture_name, std::string normal_name);
    void parse_billboard_triangle(std::stringstream& ss);
    void parse_plane(Point3D c1, Point3D c2, Point3D c3,
                     std::string material_name, std::string texture_name);
    void parse_plane(std::stringstream &ss);
    void parse_billboard_plane(std::stringstream &ss);
    void parse_textured_plane(Vector3D c1, Vector3D c2, Vector3D c3,
                                Point2D tile, std::string material_name, std::string texture_name);
    void parse_textured_plane(std::stringstream &ss);
    void parse_normal_plane(Vector3D c1, Vector3D c2, Vector3D c3,
                            Vector2D tile, std::string material_name, std::string texture_name,
                            std::string normal_name);
    void parse_normal_plane(std::stringstream &ss);
    std::vector<Point3D> parse_box(Vector3D center, Vector3D extents, Vector3D rotations);
    void parse_box(std::stringstream &ss);
    void parse_textured_box(std::stringstream &ss);
    void parse_normal_box(std::stringstream &ss);
    void parse_fog_box(std::stringstream &ss);
    void parse_fog_sphere(std::stringstream &ss);
    void parse_mesh(std::stringstream &ss);
    void parse_obj(std::stringstream& ss);
    void parse_output(std::stringstream &ss);
    void parse_rays(std::stringstream& ss);
    void parse_material(std::stringstream &ss);
    void parse_texture(std::stringstream &ss);
    void parse_normal(std::stringstream &ss);
    void parse_specular(std::stringstream &ss);
    void parse_threads(std::stringstream &ss);
    void parse_sun(std::stringstream &ss);
    void parse_sky(std::stringstream &ss);
    void parse_skysphere(std::stringstream &ss);
    void parse_checkpoints(std::stringstream &ss);
    Material *get_material(std::string name);
    Texture* get_texture(std::string name);
    Normal* get_normal(std::string name);
    std::shared_ptr<PropertyMap> get_specular(std::string name);
    std::shared_ptr<PropertyMap> get_properties(std::string material_name, std::string texture_name);
};

void remove_comment(std::string& line);
bool is_whitespace(const std::string& line);