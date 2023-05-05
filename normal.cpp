#include "normal.h"
#include "point2d.h"
#include "vector3d.h"
#include <vector>
#include <math.h>

Normal::Normal(const Vector3D& inverted)
    :inverted{inverted} {}

FlatNormal::FlatNormal(const Vector3D& inverted)
    :Normal{inverted} {}

Vector3D FlatNormal::get_vector(const Point2D&) const {
    return Vector3D{0, 0, 0};
}

NormalMap::NormalMap(const Vector3D& inverted, std::vector<int> image, unsigned width, unsigned height)
    :Normal{inverted}, image{image}, width{width}, height{height} {}

Vector3D NormalMap::get_vector(const Point2D& uv) const  {
    int x = static_cast<int>(uv.x * width);
    int y = static_cast<int>(uv.y * height);
    int index = y * width + x;
    double r, g, b;
    r = image[index * 4] - 127.5;
    g = image[index * 4 + 1] - 127.5;
    b = image[index * 4 + 2] - 127.5;
    Vector3D c{r, g, b};
    c /= 127.5;
    return c * inverted;
}

Golfball::Golfball(const Vector3D &inverted)
    :Normal{inverted} {}

Vector3D Golfball::get_vector(const Point2D& uv) const {
    double x = 0.999 - fmod(uv.x * 10, 1.0) * 2;
    return Vector3D{x, 0.0, 0.0};
}