#pragma once

#include <vector>
#include "vector3d.h"
#include "point2d.h"
#include <optional>

class Normal {
public:
    Normal(const Vector3D inverted);
    virtual ~Normal(){};
    virtual std::optional<Vector3D> get_vector(const Point2D &uv) const = 0;

    const Vector3D inverted;
};

class FlatNormal : public Normal {
public:
    FlatNormal(const Vector3D inverted);
    std::optional<Vector3D> get_vector(const Point2D &uv) const override;
};

class NormalMap : public Normal {
public:
    NormalMap(const Vector3D inverted, std::vector<int> image, unsigned width, unsigned height);
    std::optional<Vector3D> get_vector(const Point2D& uv) const override;

    std::vector<int> image;
    unsigned width, height;
};

class Golfball : public Normal {
public:
    Golfball(const Vector3D inverted);
    std::optional<Vector3D> get_vector(const Point2D &uv) const override;
};