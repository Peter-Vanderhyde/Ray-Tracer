#include "property_map.h"


PropertyMap::PropertyMap(Material* default_material, Texture* default_texture)
    : default_material{default_material}, default_texture{default_texture} {}

Material* PropertyMap::get_material(double, double) const {
    return default_material;
}

Texture* PropertyMap::get_texture(double, double) const {
    return default_texture;
}

SpecularMap::SpecularMap(Material* default_material, Texture* default_texture, Material* second_material,
                        Texture* second_texture, std::vector<int> image, unsigned width, unsigned height)
    : PropertyMap{default_material, default_texture}, second_material{second_material},
    second_texture{second_texture}, image{image}, width{width}, height{height} {}

Material* SpecularMap::get_material(double u, double v) const {
    int x = static_cast<int>(u * (width - 1));
    int y = static_cast<int>(v * (height - 1));
    int index = y * width + x;
    double r, g, b;
    r = static_cast<double>(image[index * 4]) / 255.0;
    g = static_cast<double>(image[index * 4 + 1]) / 255.0;
    b = static_cast<double>(image[index * 4 + 2]) / 255.0;
    if (r > 0.5 && g > 0.5 && b > 0.5) {
        return default_material;
    } else {
        return second_material;
    }
}

Texture* SpecularMap::get_texture(double u, double v) const {
    int x = static_cast<int>(u * (width - 1));
    int y = static_cast<int>(v * (height - 1));
    int index = y * width + x;
    double r, g, b;
    r = static_cast<double>(image[index * 4]) / 255.0;
    g = static_cast<double>(image[index * 4 + 1]) / 255.0;
    b = static_cast<double>(image[index * 4 + 2]) / 255.0;
    if (r > 0.5 && g > 0.5 && b > 0.5) {
        return default_texture;
    } else {
        return second_texture;
    }
}