#pragma once

#include <optional>
#include "texture.h"
#include "point2d.h"

class Hit;
class Ray;

class Skysphere {
public:
    Skysphere(Point2D tile, Texture* texture);

    // return the distance if ray intersects
    std::optional<double> intersect(const Ray& ray) const; // doesn't change the object
    Point2D uv(const Ray& ray, double time) const;

    Vector3D center{0, 0, 0};
    Point2D tile;
    double radius {10000000};
    Texture* texture;
};