#pragma once

#include "color.h"
#include "pixels.h"

class Material;
class Specular;

class Texture {
public:
    Texture(std::string name, const Color &color);
    virtual ~Texture() {};
    virtual Color uv(double u, double v) const = 0;
    virtual double opacity(double u, double v) const;

    std::string name;

protected:
    Color color;
};


class Solid : public Texture {
public:
    Solid(Color color);
    Color uv(double u, double v) const override;
};

class Gradient : public Texture {
public:
    Gradient(Color color, Color secondary);
    Color uv(double u, double v) const override;

    Color secondary;
};

class Dots : public Texture {
public:
    Dots(Color color);
    Color uv(double u, double v) const override;
};

class Swirl : public Texture {
public:
    Swirl(Color color, Color secondary, double num_of_stripes, double width_percent);
    Color uv(double u, double v) const override;

    Color secondary;
    double num_of_stripes, width_percent;
};

class Squares : public Texture {
public:
    Squares(Color color, Color secondary);
    Color uv(double u, double v) const override;

    Color secondary;
};

class Checkered : public Texture {
public:
    Checkered(Color color, Color secondary);
    Color uv(double u, double v) const override;

    Color secondary;
};

class Image : public Texture {
public:
    Image(std::vector<int> image, unsigned width, unsigned height);
    Color uv(double u, double v) const override;
    double opacity(double u, double v) const override;

    std::vector<int> image;
    unsigned width, height;
};

class SpecularTexture : public Texture {
public:
    SpecularTexture(Texture* texture, Material* material);
    Color uv(double u, double v) const override;

    Texture* texture;
    Specular* spec_material;
};

Color mix(double mixFactor, const Color& color1, const Color& color2);