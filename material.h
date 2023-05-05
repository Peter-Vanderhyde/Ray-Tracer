#pragma once

#include "color.h"
#include "ray.h"
#include "vector3d.h"
#include "texture.h"
#include <memory>

class Hit;

class Material {
public:
    Material(std::string name, bool emitting);
    Material(std::string name, bool emitting, double index_ratio);
    virtual ~Material() {}

    virtual Ray scatter(const Ray &ray, const Hit &hit) const = 0;

    std::string name;
    bool emitting;
    double index_ratio;
};

class Diffuse : public Material {
public:
    Diffuse(bool emitting);

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};

class Specular : public Material {
public:
    Specular(bool emitting);

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};

class Metal : public Material {
public:
    Metal(bool emitting, double fuzz);

    Ray scatter(const Ray &ray, const Hit &hit) const override;

private:
    // radius of sphere of uncertainty in reflection direction
    double fuzz;
};

class Glass : public Material {
public:
    Glass(bool emitting, double index_ratio);

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};

class Gloss : public Material {
public:
    Gloss(bool emitting);

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};

class Foggy : public Material {
public:
    Foggy();

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};

Vector3D reflect(const Vector3D &vector, const Vector3D &normal);

Vector3D refract(const Vector3D &vector, const Vector3D &normal, double index_ratio);

double schlick(double cos_theta, double index_ratio);