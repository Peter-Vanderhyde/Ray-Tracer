#pragma once

#include "material.h"

class Hit;

class Diffuse : public Material {
public:
    Diffuse(Color color, bool emitting);

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};