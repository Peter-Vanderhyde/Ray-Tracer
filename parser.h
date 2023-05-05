#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <memory>

#include "camera.h"
#include "pixels.h"
#include "world.h"

class Material;

using Materials = std::map<std::string, std::shared_ptr<Material>>;

class Parser {
public:
    Parser(const std::string& filename);
    Camera get_camera();
    World get_world();
    Pixels get_pixels();
    std::string get_output_filename();

    int depth, samples;
    int threads{1};

private:
    std::string input_filename;
    bool found_camera, found_pixels, found_output, found_rays;
    Point3D camera_position, camera_target;
    Vector3D camera_up;
    double camera_fov, aspect;
    int columns, rows;
    World world;
    std::string output_filename;
    Materials materials;

    void parse(std::ifstream& input);
    void verify();
    void parse_camera(std::stringstream& ss);
    void parse_pixels(std::stringstream& ss);
    void parse_sphere(std::stringstream& ss);
    void parse_output(std::stringstream& ss);
    void parse_rays(std::stringstream& ss);
    void parse_material(std::stringstream &ss);
    void parse_threads(std::stringstream &ss);
};

void remove_comment(std::string& line);
bool is_whitespace(const std::string& line);
