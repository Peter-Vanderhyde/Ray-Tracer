#pragma once

#include <vector>
#include "vector3d.h"
#include "point2d.h"
#include <optional>

class Normal {
public:
    Normal(const Vector3D inverted);
    virtual ~Normal(){};
    virtual Vector3D get_vector(const Point2D &uv, const Vector3D& hit_normal) const = 0;

    const Vector3D inverted;
};

class FlatNormal : public Normal {
public:
    FlatNormal(const Vector3D inverted);
    Vector3D get_vector(const Point2D &uv, const Vector3D& hit_normal) const override;
};

class NormalMap : public Normal {
public:
    NormalMap(const Vector3D inverted, std::vector<int> image, unsigned width, unsigned height);
    Vector3D get_vector(const Point2D& uv, const Vector3D& hit_normal) const override;
    void adjust_normal(Vector3D& map_normal, const Vector3D& hit_normal) const;

    std::vector<int> image;
    unsigned width, height;
};

class Golfball : public Normal {
public:
    Golfball(const Vector3D inverted);
    Vector3D get_vector(const Point2D &uv, const Vector3D& hit_normal) const override;
};