#pragma once

#include "material.h"

class PropertyMap {
public:
    PropertyMap(Material* default_material, Texture* default_texture);
    virtual Material* get_material(double u, double v) const;
    virtual Texture* get_texture(double u, double v) const;

    Material* default_material;
    Texture* default_texture;
};

class SpecularMap : public PropertyMap {
public:
    SpecularMap(Material* default_material, Texture* default_texture, Material* second_material,
                Texture* second_texture, std::vector<int> image, unsigned width, unsigned height);
    Material* get_material(double u, double v) const override;
    Texture* get_texture(double u, double v) const override;

    Material* second_material;
    Texture* second_texture;
    std::vector<int> image;
    unsigned width, height;
};