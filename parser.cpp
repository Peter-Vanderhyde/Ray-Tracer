#include "parser.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "material.h"
#include "texture.h"
#include "sphere.h"
#include <fstream>
#include <vector>
#include <string>
#include "lodepng.h"
#include "triangle.h"
#include "constants.h"
#include "vector2d.h"
#include "point3d.h"
#include "triangleobj.h"
#include "trianglebillboard.h"
#include "fogbox.h"
#include "fogsphere.h"
#include "normal.h"
#include "random.h"

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

std::optional<Sun> Parser::get_sun() {
    if (found_sun) {
        return Sun(sun_direction, sun_color, sun_intensity);
    }
    else {
        return {};
    }
}

bool Parser::has_sky() {
    return found_sky;
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
        else if (type == "normal_sphere") {
            parse_normal_sphere(ss);
        }
        else if (type == "triangle") {
            parse_triangle(ss);
        }
        else if (type == "textured_triangle") {
            parse_textured_triangle(ss);
        }
        else if (type == "normal_triangle") {
            parse_normal_triangle(ss);
        }
        else if (type == "billboard_triangle") {
            parse_billboard_triangle(ss);
        }
        else if (type == "plane") {
            parse_plane(ss);
        }
        else if (type == "billboard_plane") {
            parse_billboard_plane(ss);
        }
        else if (type == "textured_plane") {
            parse_textured_plane(ss);
        }
        else if (type == "normal_plane") {
            parse_normal_plane(ss);
        }
        else if (type == "box") {
            parse_box(ss);
        }
        else if (type == "textured_box") {
            parse_textured_box(ss);
        }
        else if (type == "normal_box") {
            parse_normal_box(ss);
        }
        else if (type == "fog_box") {
            parse_fog_box(ss);
        }
        else if (type == "fog_sphere") {
            parse_fog_sphere(ss);
        }
        else if (type == "mesh") {
            parse_mesh(ss);
        }
        else if (type == "obj") {
            parse_obj(ss);
        }
        else if (type == "output") {
            parse_output(ss);
        }
        else if (type == "rays") {
            parse_rays(ss);
        }
        else if (type == "material") {
            parse_material(ss);
        }
        else if (type == "texture") {
            parse_texture(ss);
        }
        else if (type == "normal") {
            parse_normal(ss);
        }
        else if (type == "threads") {
            parse_threads(ss);
        }
        else if (type == "sun") {
            parse_sun(ss);
        }
        else if (type == "sky") {
            parse_sky(ss);
        }
        else {
            std::string msg{"Unrecognized type keyword in line: " + line};
            throw std::runtime_error(msg);
        }
    }
}

void Parser::verify() {
    if (!found_camera) {
        throw std::runtime_error(input_filename + " is missing camera.");
    }
    if (!found_pixels) {
        throw std::runtime_error(input_filename + " is missing pixels.");
    }
    if (output_filename.empty()) {
        throw std::runtime_error(input_filename + " is missing output.");
    }
    if (world.objects.empty()) {
        throw std::runtime_error(input_filename + " is missing a shape.");
    }
    if (!found_rays) {
        throw std::runtime_error(input_filename + " is missing rays.");
    }
    if (threads < 1) {
        throw std::runtime_error(input_filename + " must be at least 1 core.");
    }
    if (threads > samples) {
        throw std::runtime_error(input_filename + " cannot use more threads than the number of ray samples.");
    }
}

void Parser::parse_camera(std::stringstream& ss) {
    if (ss >> camera_position >> camera_target >> camera_up >> camera_fov) {
        found_camera = true;
    }
    else {
        throw std::runtime_error(input_filename + " has malformed camera.");
    }
}

void Parser::parse_pixels(std::stringstream& ss) {
    if (ss >> columns >> rows) {
        found_pixels = true;
        aspect = static_cast<double>(columns) / rows;
    }
    else {
        throw std::runtime_error(input_filename + " has malformed pixels.");
    }
}

void Parser::parse_sphere(std::stringstream& ss) {
    Vector3D center;
    double radius;
    Point2D tile;
    std::string material_name, texture_name;
    ss >> center >> radius >> tile >> material_name >> texture_name;

    Material* material{get_material(material_name)};
    Texture* texture{get_texture(texture_name)};
    Normal *normal{get_normal("generic")};
    world.add(std::make_shared<Sphere>(center, radius, tile, material, texture, normal));
}

void Parser::parse_normal_sphere(std::stringstream& ss) {
    Vector3D center;
    double radius;
    Point2D tile;
    std::string material_name, texture_name, normal_name;
    if (!(ss >> center >> radius >> tile >> material_name >> texture_name >> normal_name)) {
        throw std::runtime_error("Missing arg for normal sphere (center radius tilingXY material texture normal).");
    }

    Material* material{get_material(material_name)};
    Texture* texture{get_texture(texture_name)};
    Normal *normal{get_normal(normal_name)};
    world.add(std::make_shared<Sphere>(center, radius, tile, material, texture, normal));
}

void Parser::parse_triangle(std::stringstream& ss) {
    Vector3D v0, v1, v2;
    std::string material_name, texture_name;
    ss >> v0 >> v1 >> v2 >> material_name >> texture_name;
    
    Material* material{get_material(material_name)};
    Texture* texture{get_texture(texture_name)};
    Normal *normal{get_normal("generic")};
    world.add(std::make_shared<Triangle>(v0, v1, v2, material, texture, normal));
}

void Parser::parse_textured_triangle(std::stringstream& ss) {
    Vector3D v0, v1, v2;
    Point2D t0, t1, t2;
    std::string material_name, texture_name;
    if (ss >> v0 >> v1 >> v2 >> t0 >> t1 >> t2 >> material_name >> texture_name) {
        parse_textured_triangle(v0, v1, v2, t0, t1, t2, material_name, texture_name, "generic");
    }
    else {
        throw std::runtime_error("Malformed textured triangle (v1 v2 v3 t1 t2 t3 material texture).");
    }
}

void Parser::parse_textured_triangle(Vector3D v0, Vector3D v1, Vector3D v2,
                                Point2D t0, Point2D t1, Point2D t2, std::string material_name, std::string texture_name,
                                std::string normal_name) {
    
    Material* material{get_material(material_name)};
    Texture* texture{get_texture(texture_name)};
    Normal *normal{get_normal(normal_name)};
    world.add(std::make_shared<TriangleObj>(v0, v1, v2, t0, t1, t2, material, texture, normal));
}

void Parser::parse_normal_triangle(std::stringstream& ss) {
    Vector3D v0, v1, v2;
    Point2D n0, n1, n2;
    std::string material_name, texture_name, normal_name;
    if (ss >> v0 >> v1 >> v2 >> n0 >> n1 >> n2 >> material_name >> texture_name >> normal_name)
    {
        parse_textured_triangle(v0, v1, v2, n0, n1, n2, material_name, texture_name, normal_name);
    }
    else {
        throw std::runtime_error("Malformed normal triangle (v1 v2 v3 n1 n2 n3 material texture normal)");
    }
}

void Parser::parse_billboard_triangle(std::stringstream& ss) {
    Vector3D v0, v1, v2;
    std::string material_name, texture_name;
    if (ss >> v0 >> v1 >> v2 >> material_name >> texture_name) {
        parse_billboard_triangle(v0, v1, v2, material_name, texture_name, "generic");
    }
    else {
        throw std::runtime_error("Malformed billboard triangle (v1 v2 v3 material texture).");
    }
}

void Parser::parse_billboard_triangle(Vector3D v0, Vector3D v1, Vector3D v2,
                                std::string material_name, std::string texture_name,
                                std::string normal_name) {
    
    Material* material{get_material(material_name)};
    Texture* texture{get_texture(texture_name)};
    Normal *normal{get_normal(normal_name)};
    world.add(std::make_shared<TriangleBillboard>(v0, v1, v2, material, texture, normal));
}

void Parser::parse_plane(std::stringstream& ss) {
    Vector3D c1, c2, c3, c4, center;
    std::string material_name, texture_name;
    ss >> c1 >> c2 >> c3 >> material_name >> texture_name;

    c4 = c3 + (c1 - c2);

    Material* material{get_material(material_name)};
    Texture* texture{get_texture(texture_name)};
    Normal *normal{get_normal("generic")};

    world.add(std::make_shared<Triangle>(c1, c2, c3, material, texture, normal));
    world.add(std::make_shared<Triangle>(c1, c4, c3, material, texture, normal));
}

void Parser::parse_billboard_plane(std::stringstream& ss) {
    Vector3D c1, c2, c3, c4, center;
    std::string material_name, texture_name;
    ss >> c1 >> c2 >> c3 >> material_name >> texture_name;

    c4 = c3 + (c1 - c2);

    Material* material{get_material(material_name)};
    Texture* texture{get_texture(texture_name)};
    Normal *normal{get_normal("generic")};

    world.add(std::make_shared<TriangleBillboard>(c1, c3, c2, material, texture, normal));
    world.add(std::make_shared<TriangleBillboard>(c1, c4, c3, material, texture, normal));
}

void Parser::parse_plane(Point3D c1, Point3D c2, Point3D c3,
                        std::string material_name, std::string texture_name) {
    Vector3D c4, center;
    c4 = c3 + (c1 - c2);

    Material* material{get_material(material_name)};
    Texture* texture{get_texture(texture_name)};
    Normal *normal{get_normal("generic")};

    world.add(std::make_shared<Triangle>(c1, c2, c3, material, texture, normal));
    world.add(std::make_shared<Triangle>(c1, c4, c3, material, texture, normal));
}

void Parser::parse_textured_plane(std::stringstream& ss) {
    Vector3D c1, c2, c3, c4;
    Point2D tile;
    std::string material_name, texture_name;
    if (ss >> c1 >> c2 >> c3 >> tile >> material_name >> texture_name) {
        c4 = c3 + (c1 - c2);

        Material* material{get_material(material_name)};
        Texture* texture{get_texture(texture_name)};
        Normal *normal{get_normal("generic")};

        world.add(std::make_shared<TriangleObj>(c1, c2, c3, Point2D{0, 0}, Point2D{0, tile.y},
                                                Point2D{tile.x, tile.y}, material, texture, normal));
        world.add(std::make_shared<TriangleObj>(c1, c4, c3, Point2D{0, 0}, Point2D{tile.x, 0},
                                                Point2D{tile.x, tile.y}, material, texture, normal));
    }
    else {
        throw std::runtime_error("Malformed textured plane (v1 v2 v3 tile material texture).");
    }
}

void Parser::parse_textured_plane(Vector3D c1, Vector3D c2, Vector3D c3,
                                Point2D tile, std::string material_name, std::string texture_name) {
    Vector3D c4;

    c4 = c3 + (c1 - c2);

    Material* material{get_material(material_name)};
    Texture* texture{get_texture(texture_name)};
    Normal *normal{get_normal("generic")};

    world.add(std::make_shared<TriangleObj>(c1, c2, c3, Point2D{0, 0}, Point2D{0, tile.y},
                                            Point2D{tile.x, tile.y}, material, texture, normal));
    world.add(std::make_shared<TriangleObj>(c1, c4, c3, Point2D{0, 0}, Point2D{tile.x, 0},
                                            Point2D{tile.x, tile.y}, material, texture, normal));
}

void Parser::parse_normal_plane(std::stringstream& ss) {
    Vector3D c1, c2, c3, c4;
    Point2D tile;
    std::string material_name, texture_name, normal_name;
    if (ss >> c1 >> c2 >> c3 >> tile >> material_name >> texture_name >> normal_name)
    {
        c4 = c3 + (c1 - c2);

        Material* material{get_material(material_name)};
        Texture* texture{get_texture(texture_name)};
        Normal *normal{get_normal(normal_name)};

        world.add(std::make_shared<TriangleObj>(c1, c2, c3, Point2D{0, 0}, Point2D{0, tile.y},
                                                Point2D{tile.x, tile.y}, material, texture, normal));
        world.add(std::make_shared<TriangleObj>(c1, c4, c3, Point2D{0, 0}, Point2D{tile.x, 0},
                                                Point2D{tile.x, tile.y}, material, texture, normal));
    }
    else {
        throw std::runtime_error("Malformed textured plane (v1 v2 v3 tile material texture).");
    }
}

void Parser::parse_normal_plane(Vector3D c1, Vector3D c2, Vector3D c3,
                                Point2D tile, std::string material_name, std::string texture_name,
                                std::string normal_name) {
    Vector3D c4;

    c4 = c3 + (c1 - c2);

    Material* material{get_material(material_name)};
    Texture* texture{get_texture(texture_name)};
    Normal *normal{get_normal(normal_name)};

    world.add(std::make_shared<TriangleObj>(c1, c2, c3, Point2D{0, 0}, Point2D{0, tile.y},
                                            Point2D{tile.x, tile.y}, material, texture, normal));
    world.add(std::make_shared<TriangleObj>(c1, c4, c3, Point2D{0, 0}, Point2D{tile.x, 0},
                                            Point2D{tile.x, tile.y}, material, texture, normal));
}

void Parser::parse_box(std::stringstream& ss) {
    Vector3D center, extents, rotations;
    std::string material_name, texture_name;
    ss >> center >> extents >> rotations >> material_name >> texture_name;

    if (extents.x <= 0.0 || extents.y <= 0.0 || extents.z <= 0.0) {
        throw std::runtime_error("Box must have length in all directions.");
    }
    double half_x = extents.x / 2;
    double half_y = extents.y / 2;
    double half_z = extents.z / 2;
    // front, left, back, right, top, bot
    std::vector<int> x_signs{-1, -1, 1, -1, -1, -1, 1, 1, -1, 1, 1, 1, -1, -1, 1, 1, 1, -1};
    std::vector<int> y_signs{-1, -1, -1, 1, 1, -1, 1, 1, 1, -1, -1, 1, 1, -1, -1, 1, -1, -1};
    std::vector<int> z_signs{1, -1, -1, 1, -1, -1, 1, -1, -1, 1, -1, -1, 1, 1, 1, -1, -1, -1};
    Point3D c1, c2, c3;
    for (size_t i = 0; i < x_signs.size() / 3; ++i) {
        c1 = {half_x * x_signs[3 * i], half_y * y_signs[3 * i], half_z * z_signs[3 * i]};
        c2 = {half_x * x_signs[3 * i + 1], half_y * y_signs[3 * i + 1], half_z * z_signs[3 * i + 1]};
        c3 = {half_x * x_signs[3 * i + 2], half_y * y_signs[3 * i + 2], half_z * z_signs[3 * i + 2]};
        c1.rotate(rotations);
        c2.rotate(rotations);
        c3.rotate(rotations);
        c1 += center;
        c2 += center;
        c3 += center;
        parse_plane(c1, c2, c3, material_name, texture_name);
    }
}

void Parser::parse_textured_box(std::stringstream& ss) {
    Vector3D center, extents, rotations;
    Point2D tile;
    std::string material_name, texture_name;
    ss >> center >> extents >> rotations >> tile >> material_name >> texture_name;

    if (extents.x <= 0.0 || extents.y <= 0.0 || extents.z <= 0.0) {
        throw std::runtime_error("Box must have length in all directions.");
    }
    double half_x = extents.x / 2;
    double half_y = extents.y / 2;
    double half_z = extents.z / 2;
    // front, left, back, right, top, bot
    std::vector<int> x_signs{-1, -1, 1, -1, -1, -1, 1, 1, -1, 1, 1, 1, -1, -1, 1, 1, 1, -1};
    std::vector<int> y_signs{-1, -1, -1, 1, 1, -1, 1, 1, 1, -1, -1, 1, 1, -1, -1, 1, -1, -1};
    std::vector<int> z_signs{1, -1, -1, 1, -1, -1, 1, -1, -1, 1, -1, -1, 1, 1, 1, -1, -1, -1};
    Point3D c1, c2, c3;
    for (size_t i = 0; i < x_signs.size() / 3; ++i) {
        c1 = {half_x * x_signs[3 * i], half_y * y_signs[3 * i], half_z * z_signs[3 * i]};
        c2 = {half_x * x_signs[3 * i + 1], half_y * y_signs[3 * i + 1], half_z * z_signs[3 * i + 1]};
        c3 = {half_x * x_signs[3 * i + 2], half_y * y_signs[3 * i + 2], half_z * z_signs[3 * i + 2]};
        c1.rotate(rotations);
        c2.rotate(rotations);
        c3.rotate(rotations);
        c1 += center;
        c2 += center;
        c3 += center;
        parse_textured_plane(c1, c2, c3, tile, material_name, texture_name);
    }
}

void Parser::parse_normal_box(std::stringstream& ss) {
    Vector3D center, extents, rotations;
    Point2D tile;
    std::string material_name, texture_name, normal_name;
    ss >> center >> extents >> rotations >> tile >> material_name >> texture_name >> normal_name;

    if (extents.x <= 0.0 || extents.y <= 0.0 || extents.z <= 0.0) {
        throw std::runtime_error("Box must have length in all directions.");
    }
    double half_x = extents.x / 2;
    double half_y = extents.y / 2;
    double half_z = extents.z / 2;
    // front, left, back, right, top, bot
    std::vector<int> x_signs{-1, -1, 1, -1, -1, -1, 1, 1, -1, 1, 1, 1, -1, -1, 1, 1, 1, -1};
    std::vector<int> y_signs{-1, -1, -1, 1, 1, -1, 1, 1, 1, -1, -1, 1, 1, -1, -1, 1, -1, -1};
    std::vector<int> z_signs{1, -1, -1, 1, -1, -1, 1, -1, -1, 1, -1, -1, 1, 1, 1, -1, -1, -1};
    Point3D c1, c2, c3;
    for (size_t i = 0; i < x_signs.size() / 3; ++i) {
        c1 = {half_x * x_signs[3 * i], half_y * y_signs[3 * i], half_z * z_signs[3 * i]};
        c2 = {half_x * x_signs[3 * i + 1], half_y * y_signs[3 * i + 1], half_z * z_signs[3 * i + 1]};
        c3 = {half_x * x_signs[3 * i + 2], half_y * y_signs[3 * i + 2], half_z * z_signs[3 * i + 2]};
        c1.rotate(rotations);
        c2.rotate(rotations);
        c3.rotate(rotations);
        c1 += center;
        c2 += center;
        c3 += center;
        parse_normal_plane(c1, c2, c3, tile, material_name, texture_name, normal_name);
    }
}

void Parser::parse_fog_box(std::stringstream& ss) {
    Vector3D center, dimensions, rotations;
    double density;
    std::string material_name, texture_name;
    ss >> center >> dimensions >> rotations >> density >> material_name >> texture_name;
    
    Material* material{get_material(material_name)};
    Texture* texture{get_texture(texture_name)};
    Normal *normal{get_normal("generic")};
    world.add(std::make_shared<FogBox>(center, dimensions, rotations, density, material, texture, normal));
}

void Parser::parse_fog_sphere(std::stringstream& ss) {
    Vector3D center;
    double radius, density;
    std::string material_name, texture_name;
    ss >> center >> radius >> density >> material_name >> texture_name;

    Material *material{get_material(material_name)};
    Texture *texture{get_texture(texture_name)};
    Normal *normal{get_normal("generic")};
    world.add(std::make_shared<FogSphere>(center, radius, density, material, texture, normal));
}

void Parser::parse_mesh(std::stringstream& ss) {
    Vector3D position, rotations, scales;
    std::string filename, material_name, texture_name;
    if (!(ss >> position >> filename >> scales >> rotations >> material_name >> texture_name)) {
        throw std::runtime_error("Malformed mesh line (position filename scaling rotations material texture).");
    }

    Material* material = get_material(material_name);
    Texture* texture = get_texture(texture_name);
    Normal *normal{get_normal("generic")};

    std::ifstream input{"files/meshes/" + filename};
    if (!input) {
        throw std::runtime_error("Cannot open mesh file: " + filename);
    }
    std::string temp;
    input >> temp;
    if (temp != "vertices") {
        throw std::runtime_error("Mesh file must start with string 'vertices'.");
    }

    std::vector<Vector3D> vertices;
    Vector3D min_coord{0, 0, 0}, max_coord{0, 0, 0};
    for (Vector3D vertex; input >> vertex;)
    {
        vertex.scale(scales);
        if (vertex.x > max_coord.x) {
            max_coord.x = vertex.x;
        }
        if (vertex.y > max_coord.y) {
            max_coord.y = vertex.y;
        }
        if (vertex.z > max_coord.z) {
            max_coord.z = vertex.z;
        }
        if (vertex.x < min_coord.x) {
            min_coord.x = vertex.x;
        }
        if (vertex.y < min_coord.y) {
            min_coord.y = vertex.y;
        }
        if (vertex.z < min_coord.z) {
            min_coord.z = vertex.z;
        }
        vertex.rotate(rotations);
        vertices.push_back(vertex + position);
    }
    if (vertices.size() < 3) {
        throw std::runtime_error("Mesh file must contain at least 3 vertices.");
    }
    std::cout << filename << " Scale: " << max_coord - min_coord << '\n';

    input.clear(); // clear error bit because "triangles" was read
    input >> temp; // read "triangles"

    // read each triangle line
    for (int a, b, c; input >> a >> b >> c;) {
        world.add(std::make_shared<Triangle>(vertices.at(a), vertices.at(b), vertices.at(c), material, texture, normal));
    }
}

void Parser::parse_obj(std::stringstream& ss) {
    Vector3D position, rotations;
    double scale;
    std::string filename, material_name, texture_name;
    int sections;
    if (!(ss >> position >> filename >> sections >> scale >> rotations >> material_name >> texture_name))
    {
        throw std::runtime_error("Malformed obj line. Should be:\nPosition, filename, sections, scale, rotations, material, texture.");
    }
    Material* material = get_material(material_name);
    Texture* texture = get_texture(texture_name);
    Normal *normal{get_normal("generic")};

    std::ifstream input{"files/objs/" + filename};
    if (!input) {
        throw std::runtime_error("Cannot open obj file: " + filename);
    }
    std::string temp;
    std::vector<Vector3D> vertices;
    Vector3D max_coord{0,0,0}, min_coord{0,0,0};
    std::vector<Vector2D> texture_vertices;
    std::vector<std::vector<std::pair<int, int>>> faces;
    std::vector<std::pair<int, int>> face;
    while (input >> temp)
    {
        if (temp == "o") {
            if (sections == 0) {
                break;
            }
            else {
                sections -= 1;
                std::string section;
                input >> section;
                std::cout << "Parsing " << section << '\n';
            }
        }
        else if (temp == "v") {
            double x, y, z;
            input >> x >> y >> z;
            Vector3D vertex{x, y, z};
            vertex *= scale;
            vertex.rotate(rotations);
            if (vertex.x > max_coord.x) {
                max_coord.x = vertex.x;
            }
            if (vertex.y > max_coord.y) {
                max_coord.y = vertex.y;
            }
            if (vertex.z > max_coord.z) {
                max_coord.z = vertex.z;
            }
            if (vertex.x < min_coord.x) {
                min_coord.x = vertex.x;
            }
            if (vertex.y < min_coord.y) {
                min_coord.y = vertex.y;
            }
            if (vertex.z < min_coord.z) {
                min_coord.z = vertex.z;
            }
            vertices.push_back(vertex + position);
        }
        else if (temp == "vt") {
            double x, y;
            input >> x >> y;
            Vector2D uv{x, y};
            texture_vertices.push_back(uv);
        }
        else if (temp == "f") {
            if (face.size() != 0) {
                faces.push_back(face);
            }
            face = {};
        }
        else if (temp.find('/') != temp.npos) {
            int vertex_id;
            int vt_id = 0;
            std::string first = temp.substr(0, temp.find('/'));
            std::string second = temp.substr(temp.find('/') + 1);
            second = second.substr(0, second.find('/'));
            vertex_id = stoi(first);
            if (second != "") {
                vt_id = stoi(second);
            }
            face.push_back({vertex_id - 1, vt_id - 1}); // subtract because their index starts at 1
        }
    }

    if (vertices.size() < 3) {
        throw std::runtime_error("Obj file must contain at least 3 vertices.");
    }
    std::cout << filename << " Scale: " << max_coord - min_coord << '\n';

    // read each triangle line
    int invalid_triangle_count = 0;
    int triangles = 0;
    for (size_t x = 0; x < faces.size(); ++x)
    {
        triangles += faces[x].size() - 2;
        std::vector<std::pair<int, int>> face = faces[x];
        if (face.size() >= 3)
        {
            for (size_t i = 0; i < face.size() - 2; ++i){
                try{
                    if (face[0].second == -1){
                        world.add(std::make_shared<Triangle>(vertices.at(face[0].first), vertices.at(face[1 + i].first), vertices.at(face[2 + i].first),
                                                             material, texture, normal));
                    }
                    else {
                        world.add(std::make_shared<TriangleObj>(vertices.at(face[0].first), vertices.at(face[1 + i].first), vertices.at(face[2 + i].first),
                                                             texture_vertices.at(face[0].second), texture_vertices.at(face[i + 1].second), texture_vertices.at(face[i + 2].second),
                                                             material, texture, normal));
                    }
                }
                catch (const std::exception& e) {
                    // std::cout << "Invalid " << vertices.at(face[0]) << vertices.at(face[1 + i]) << vertices.at(face[2 + i]) << '\n';
                    invalid_triangle_count += 1;
                }
            }
        }
    }
    std::cout << invalid_triangle_count << '/' << triangles << " invalid triangles\n";
}

void Parser::parse_output(std::stringstream& ss) {
    if (ss >> output_filename) {
        found_output = true;
    }
    else {
        throw std::runtime_error(input_filename + " has malformed output.");
    }
}

void Parser::parse_rays(std::stringstream& ss) {
    if (ss >> bounces >> samples) {
        found_rays = true;
    }
    else {
        throw std::runtime_error(input_filename + " has malformed rays.");
    }
}

void Parser::parse_material(std::stringstream& ss) {
    std::string name, material_name;
    ss >> name >> material_name;
    if (material_name == "light") {
        materials[name] = std::make_shared<Light>();
    }
    else if (material_name == "diffuse") {
        materials[name] = std::make_shared<Diffuse>();
    }
    else if (material_name == "specular") {
        materials[name] = std::make_shared<Specular>();
    }
    else if (material_name == "metal") {
        double fuzz;
        if (ss >> fuzz) {
            materials[name] = std::make_shared<Metal>(fuzz);
        }
        else {
            throw std::runtime_error("Missing fuzz parameter for metal.");
        }
    }
    else if (material_name == "glass") {
        double index_ratio;
        if (ss >> index_ratio) {
            materials[name] = std::make_shared<Glass>(index_ratio);
        }
        else {
            throw std::runtime_error("Missing index ratio parameter for glass.");
        }
    }
    else if (material_name == "flat_glass") {
        double index_ratio, thickness;
        if (ss >> index_ratio >> thickness) {
            materials[name] = std::make_shared<FlatGlass>(index_ratio, thickness);
        }
        else {
            throw std::runtime_error("Missing index ratio parameter for glass.");
        }
    }
    else if (material_name == "gloss") {
        double roughness;
        if (ss >> roughness) {
            materials[name] = std::make_shared<Gloss>(roughness);
        }
        else {
            throw std::runtime_error("Missing the roughness value: 0 <= roughness <= 1.");
        }
    }
    else if (material_name == "fuzzy_gloss") {
        double roughness, metalic;
        if (ss >> roughness >> metalic) {
            materials[name] = std::make_shared<FuzzyGloss>(roughness, metalic);
        }
        else {
            throw std::runtime_error("Missing either roughness or metalic value.");
        }
    }
    else if (material_name == "fog") {
        materials[name] = std::make_shared<Foggy>();
    }
    else if (material_name == "point_light") {
        materials[name] = std::make_shared<PointLight>();
    }
    else if (material_name == "directional_light") {
        double spread_angle;
        if (ss >> spread_angle) {
            materials[name] = std::make_shared<DirectionalLight>(spread_angle);
        }
        else {
            throw std::runtime_error("Missing the spread angle for directional light.");
        }
    }
    else {
        throw std::runtime_error("Unknown material: " + name);
    }
}

void Parser::parse_texture(std::stringstream& ss) {
    std::string name, texture_name;
    Color color;
    ss >> name >> texture_name;
    if (texture_name == "solid") {
        if (ss >> color){
            textures[name] = std::make_shared<Solid>(texture_name, color);
        }
        else {
            throw std::runtime_error("Missing solid color.");
        }
    }
    else if (texture_name == "gradient") {
        Color secondary;
        if (ss >> color >> secondary)
        {
            textures[name] = std::make_shared<Gradient>(texture_name, color, secondary);
        }
        else {
            throw std::runtime_error("Malformed gradient. Need primaryColor, secondaryColor.");
        }
    }
    else if (texture_name == "dots") {
        if (ss >> color) {
            textures[name] = std::make_shared<Dots>(texture_name, color);
        }
        else {
            throw std::runtime_error("Missing dots color.");
        }
    }
    else if (texture_name == "swirl") {
        Color secondary;
        double num_of_stripes, width_percent;
        if (ss >> color >> secondary >> num_of_stripes >> width_percent)
        {
            textures[name] = std::make_shared<Swirl>(texture_name, color, secondary, num_of_stripes, width_percent);
        }
        else {
            throw std::runtime_error("Malformed swirl. Need color, secondary, num stripes, width percent.");
        }
    }
    else if (texture_name == "squares") {
        Color secondary;
        if (ss >> color >> secondary){
            textures[name] = std::make_shared<Squares>(texture_name, color, secondary);
        }
        else {
            throw std::runtime_error("Missing primary or secondary color for squares.");
        }
    }
    else if (texture_name == "checkered") {
        Color secondary;
        if (ss >> color >> secondary){
            textures[name] = std::make_shared<Checkered>(texture_name, color, secondary);
        }
        else {
            throw std::runtime_error("Missing primary or secondary color for checkered.");
        }
    }
    else if (texture_name == "image") {
        std::string image_file;
        if (ss >> image_file) {
            std::cout << "Parsing " + image_file + " texture...\n";
            std::ifstream input("files/images/" + image_file);
            if(!input) {
                throw std::runtime_error("Cannot open image file: " + image_file);
            }
            std::vector<unsigned char> image;
            unsigned width, height;
            unsigned error = lodepng::decode(image, width, height, "files/images/" + image_file);
            if (error) {
                std::cout << "Error: " + error;
            }

            Pixels pixels{static_cast<int>(width), static_cast<int>(height)};
            std::vector<int> values;
            for (int i : image) {
                values.push_back(i);
            }

            textures[name] = std::make_shared<Image>(texture_name, values, width, height);
        }
        else {
            throw std::runtime_error("Malformed image texture.");
        }
    }
    else {
        throw std::runtime_error("Unknown texture name: " + texture_name);
    }
}

void Parser::parse_normal(std::stringstream& ss) {
    std::string name, image_file;
    Vector3D inverted;
    if (ss >> name >> image_file >> inverted)
    {
        std::cout << "Parsing " + image_file + " normal...\n";
        std::ifstream input("files/normals/" + image_file);
        if(!input) {
            throw std::runtime_error("Cannot open image file: " + image_file);
        }
        std::vector<unsigned char> image;
        unsigned width, height;
        unsigned error = lodepng::decode(image, width, height, "files/normals/" + image_file);
        if (error) {
            std::cout << "Error: " + error;
        }

        Pixels pixels{static_cast<int>(width), static_cast<int>(height)};
        std::vector<int> values;
        for (int i : image) {
            values.push_back(i);
        }

        normals[name] = std::make_shared<NormalMap>(inverted, values, width, height);
    }
    else {
        throw std::runtime_error("Normal missing name or image file.");
    }
}

void Parser::parse_threads(std::stringstream& ss) {
    if (!(ss >> threads)) {
        throw std::runtime_error(input_filename + " has malformed threads.");
    }
}

void Parser::parse_sun(std::stringstream& ss) {
    if (!(ss >> sun_direction >> sun_color >> sun_intensity)) {
        throw std::runtime_error(input_filename + " the sun needs a specified direction, color, and intensity.");
    }
    else {
        found_sun = true;
        if (sun_direction.x == 3.14 && sun_direction.y == 1.59 && sun_direction.z == 2.65) {
            sun_direction = random_in_hemisphere(Vector3D{0, 0, -1});
            std::cout << "\nThe sun's direction is " << sun_direction << '\n';
        }
    }
}

void Parser::parse_sky(std::stringstream& ss) {
    if (!(ss >> std::boolalpha >> found_sky)) {
        throw std::runtime_error(input_filename + " the sky needs a specified boolean.");
    }
}

Material* Parser::get_material(std::string name) {
    auto i = materials.find(name);
    if (i != materials.end()) {
        auto material = i->second;
        return material.get();
    }
    else {
        throw std::runtime_error("Material " + name + " not defined.");
    }
}

Texture* Parser::get_texture(std::string name) {
    auto i = textures.find(name);
    if (i != textures.end()) {
        auto texture = i->second;
        return texture.get();
    }
    else {
        throw std::runtime_error("Texture " + name + " not defined.");
    }
}

Normal* Parser::get_normal(std::string name) {
    auto i = normals.find(name);
    if (i != normals.end()) {
        auto normal = i->second;
        return normal.get();
    }
    else {
        throw std::runtime_error("Normal " + name + " not defined.");
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