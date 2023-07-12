#pragma once

#include "color.h"
#include "ray.h"
#include "vector3d.h"
#include "texture.h"
#include <memory>

class Hit;

class Material {
public:
    Material(std::string name);
    Material(std::string name, double index_ratio);
    virtual ~Material() {}

    virtual Ray scatter(const Ray &ray, const Hit &hit) const = 0;

    std::string name;
    double index_ratio, spread;
    bool emitting = false;
};

class Light : public Material {
public:
    Light();

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};

class Diffuse : public Material {
public:
    Diffuse();

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};

class Specular : public Material {
public:
    Specular();

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};

class Metal : public Material {
public:
    Metal(double fuzz);

    Ray scatter(const Ray &ray, const Hit &hit) const override;

private:
    // radius of sphere of uncertainty in reflection direction
    double fuzz;
};

class Glass : public Material {
public:
    Glass(double index_ratio);

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};

class FlatGlass : public Material {
public:
    FlatGlass(double index_ratio, double thickness);

    Ray scatter(const Ray &ray, const Hit &hit) const override;

    double thickness;
};

class Gloss : public Material {
public:
    Gloss(double roughness);

    Ray scatter(const Ray &ray, const Hit &hit) const override;

    double roughness;
};

class Foggy : public Material {
public:
    Foggy();

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};

class PointLight : public Material {
public:
    PointLight();

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};

class DirectionalLight : public Material {
public:
    DirectionalLight(double spread);

    Ray scatter(const Ray &ray, const Hit &hit) const override;
};

Vector3D reflect(const Vector3D &vector, const Vector3D &normal);

Vector3D refract(const Vector3D &vector, const Vector3D &normal, double index_ratio);

double schlick(double cos_theta, double index_ratio);

double mix(double glossy, double diffuse);