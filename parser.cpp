#include "parser.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>

Parser::Parser(const std::string& filename)
    :input_filename{filename}, found_camera{false}, found_pixels{false}, found_output{false}, found_rays{false} {
    std::ifstream input{filename};
    if (!input) {
        throw std::runtime_error("Could not open " + filename);
    }
    parse(input);
    verify();
}

Camera Parser::get_camera() {
    return Camera{camera_position, camera_target, camera_up,
                  camera_fov, aspect};
}

World Parser::get_world() {
    return world;
}

Pixels Parser::get_pixels() {
    return Pixels{columns, rows};
}

std::string Parser::get_output_filename() {
    return output_filename;
}

void Parser::parse(std::ifstream& input) {
    std::string line;
    while (std::getline(input, line)) {
        remove_comment(line);
        if (is_whitespace(line)) {
            continue;
        }

        std::stringstream ss{line};
        std::string type;
        ss >> type;
        if (type == "camera") {
            parse_camera(ss);
        }
        else if (type == "pixels") {
            parse_pixels(ss);
        }
        else if (type == "sphere") {
            parse_sphere(ss);
        }
        else if (type == "output") {
            parse_output(ss);
        }
        else if (type == "rays") {
            parse_rays(ss);
        }
        else {
            std::string msg{"Unrecognized type in line: " + line};
            throw std::runtime_error(msg);
        }
    }
}

void Parser::verify() {
    if (!found_camera) {
        throw std::runtime_error(input_filename + " is missing camera");
    }
    if (!found_pixels) {
        throw std::runtime_error(input_filename + " is missing pixels");
    }
    if (output_filename.empty()) {
        throw std::runtime_error(input_filename + " is missing output");
    }
    if (world.objects.empty()) {
        throw std::runtime_error(input_filename + " is missing a sphere");
    }
    if (!found_rays) {
        throw std::runtime_error(input_filename + " is missing rays");
    }
}

void Parser::parse_camera(std::stringstream& ss) {
    if (ss >> camera_position >> camera_target >> camera_up >> camera_fov) {
        found_camera = true;
    }
    else {
        throw std::runtime_error(input_filename + " has malformed camera");
    }
}

void Parser::parse_pixels(std::stringstream& ss) {
    if (ss >> columns >> rows) {
        found_pixels = true;
        aspect = static_cast<double>(columns) / rows;
    }
    else {
        throw std::runtime_error(input_filename + " has malformed pixels");
    }
}

void Parser::parse_sphere(std::stringstream& ss) {
    Vector3D center;
    double radius;
    Color color;
    bool emitting;
    if (ss >> center >> radius >> color >> std::boolalpha >> emitting) {
        world.add(center, radius, color, emitting);
    }
    else {
        throw std::runtime_error(input_filename + " has malformed sphere");
    }
}

void Parser::parse_output(std::stringstream& ss) {
    if (ss >> output_filename) {
        found_output = true;
    }
    else {
        throw std::runtime_error(input_filename + " has malformed output");
    }
}

void Parser::parse_rays(std::stringstream& ss) {
    if (ss >> depth >> samples) {
        found_rays = true;
    }
    else {
        throw std::runtime_error(input_filename + " has malformed rays");
    }
}

void remove_comment(std::string& line) {
    auto i = line.find('#');
    if (i != std::string::npos) {
        line = line.substr(0, i);
    }
}

bool is_whitespace(const std::string& line) {
    return std::all_of(std::begin(line), std::end(line), ::isspace);
}
