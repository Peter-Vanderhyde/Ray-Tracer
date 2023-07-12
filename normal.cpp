#include "normal.h"
#include "point2d.h"
#include "vector3d.h"
#include <vector>
#include <math.h>
#include "constants.h"

Normal::Normal(const Vector3D inverted)
    :inverted{inverted} {}

FlatNormal::FlatNormal(const Vector3D inverted)
    :Normal{inverted} {}

std::optional<Vector3D> FlatNormal::get_vector(const Point2D&) const {
    return {};
}

NormalMap::NormalMap(const Vector3D inverted, std::vector<int> image, unsigned width, unsigned height)
    :Normal{inverted}, image{image}, width{width}, height{height} {}

std::optional<Vector3D> NormalMap::get_vector(const Point2D& uv) const  {
    int x = static_cast<int>(uv.x * (width - 1));
    int y = static_cast<int>(uv.y * (height - 1));
    int index = y * width + x;
    double r, g, b;
    r = static_cast<double>(image[index * 4]) / 128.0 - 1;
    g = static_cast<double>(image[index * 4 + 1]) / 128.0 - 1;
    b = static_cast<double>(image[index * 4 + 2]) / 128.0 - 1;
    Vector3D c{r, g, b};
    return c * inverted;
}

Golfball::Golfball(const Vector3D inverted)
    :Normal{inverted} {}

std::optional<Vector3D> Golfball::get_vector(const Point2D& uv) const {
    double x = 0.999 - fmod(uv.x * 10, 1.0) * 2;
    return Vector3D{x, 0.0, 0.0};
}