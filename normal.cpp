#include "normal.h"
#include "point2d.h"
#include "vector3d.h"
#include <vector>
#include <math.h>
#include "constants.h"

Normal::Normal(std::string name, const Vector3D inverted)
    :name{name}, inverted{inverted} {}

FlatNormal::FlatNormal(const Vector3D inverted)
    :Normal{"generic", inverted} {}

Vector3D FlatNormal::get_vector(const Point2D&, const Vector3D& hit_normal) const {
    return unit(hit_normal);
}

NormalMap::NormalMap(const Vector3D inverted, std::vector<int> image, unsigned width, unsigned height)
    :Normal{"normal_map", inverted}, image{image}, width{width}, height{height} {}

Vector3D NormalMap::get_vector(const Point2D& uv, const Vector3D& hit_normal) const  {
    int x = static_cast<int>(uv.x * (width - 1));
    int y = static_cast<int>(uv.y * (height - 1));
    int index = y * width + x;
    double r, g, b;
    r = static_cast<double>(image[index * 4]) / 128.0 - 1;
    g = static_cast<double>(image[index * 4 + 1]) / 128.0 - 1;
    b = static_cast<double>(image[index * 4 + 2]) / 128.0 - 1;
    Vector3D c{r, g, b};
    c *= inverted;

    // Bend the normal around the shape of the object
    adjust_normal(c, hit_normal);
    
    return c;
}

void NormalMap::adjust_normal(Vector3D& map_normal, const Vector3D& hit_normal) const {
    if (hit_normal != Vector3D{0, 0, 1} && hit_normal != Vector3D{0, 0, -1})
    {
        // Conforming the normal map vector to the shape of the object
        Vector3D bitangent = cross(Vector3D{0, 0, 1}, hit_normal);
        bitangent = unit(bitangent);
        Vector3D tangent = cross(bitangent, hit_normal);
        tangent = unit(tangent);
        map_normal = bitangent * map_normal.x + tangent * map_normal.y + hit_normal * map_normal.z;
        map_normal = unit(map_normal);
    }
    else {
        // Avoiding an error when crossing the same vector
        Vector3D bitangent = Vector3D{1, 0, 0};
        Vector3D tangent = Vector3D{0, 1, 0};
        if (hit_normal.z == -1) {
            bitangent *= -1;
        }
        map_normal = bitangent * map_normal.x + tangent * map_normal.y + hit_normal * map_normal.z;
        if (length(map_normal) == 0) {
            map_normal = unit(hit_normal);
        }
        else {
            map_normal = unit(map_normal);
        }
    }
}

Golfball::Golfball(const Vector3D inverted)
    :Normal{"golf_ball", inverted} {}

Vector3D Golfball::get_vector(const Point2D& uv, const Vector3D&) const {
    double x = 0.999 - fmod(uv.x * 10, 1.0) * 2;
    return Vector3D{x, 0.0, 0.0};
}