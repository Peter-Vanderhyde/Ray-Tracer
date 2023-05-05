#pragma once

#include "color.h"
#include "ray.h"
#include "vector3d.h"

class Hit;

class Material {
public:
    Material(std::string name, Color color, bool emitting);

    virtual Ray scatter(const Ray &ray, const Hit &hit) const = 0;

    std::string name;
    Color color;
    bool emitting;
};

class Diffuse : public Material {
public:
    Diffuse(Color color, bool emitting);

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};

class Specular : public Material {
public:
    Specular(Color color, bool emitting);

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};

class Metal : public Material {
public:
    Metal(Color color, bool emitting, double fuzz);

    Ray scatter(const Ray &ray, const Hit &hit) const override;

private:
    // radius of sphere of uncertainty in reflection direction
    double fuzz;
};

Vector3D reflect(const Vector3D &vector, const Vector3D &normal);