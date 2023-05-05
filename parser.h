#include <fstream>
#include <iostream>

#include "camera.h"
#include "pixels.h"
#include "world.h"

class Parser {
public:
    Parser(const std::string& filename);
    Camera get_camera();
    World get_world();
    Pixels get_pixels();
    std::string get_output_filename();

private:
    std::string input_filename;
    bool found_camera, found_pixels, found_output;
    Point3D camera_position, camera_target;
    Vector3D camera_up;
    double camera_fov, aspect;
    int columns, rows;
    World world;
    std::string output_filename;

    void parse(std::ifstream& input);
    void verify();
    void parse_camera(std::stringstream& ss);
    void parse_pixels(std::stringstream& ss);
    void parse_sphere(std::stringstream& ss);
    void parse_output(std::stringstream& ss);
};

void remove_comment(std::string& line);
bool is_whitespace(const std::string& line);
