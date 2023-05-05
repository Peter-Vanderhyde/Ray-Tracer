#pragma once

#include "color.h"
#include "ray.h"

class Hit;

class Material {
public:
    Material(std::string name, Color color, bool emitting);

    virtual Ray scatter(const Ray &ray, const Hit &hit) const = 0;

    std::string name;
    Color color;
    bool emitting;
};