#pragma once

#include "color.h"
#include "pixels.h"

class Texture {
public:
    Texture(std::string name, const Color &color);
    virtual ~Texture() {};
    virtual Color uv(double u, double v) const = 0;

protected:
    std::string name;
    Color color;
};


class Solid : public Texture {
public:
    Solid(std::string name, Color color);
    Color uv(double u, double v) const override;
};

class Gradient : public Texture {
public:
    Gradient(std::string name, Color color);
    Color uv(double u, double v) const override;
};

class Dots : public Texture {
public:
    Dots(std::string name, Color color);
    Color uv(double u, double v) const override;
};

class Swirl : public Texture {
public:
    Swirl(std::string name, Color color, Color secondary, double num_of_stripes, double width_percent);
    Color uv(double u, double v) const override;

    Color secondary;
    double num_of_stripes, width_percent;
};

class Squares : public Texture {
public:
    Squares(std::string name, Color color, Color secondary);
    Color uv(double u, double v) const override;

    Color secondary;
};

class Checkered : public Texture {
public:
    Checkered(std::string name, Color color, Color secondary);
    Color uv(double u, double v) const override;

    Color secondary;
};

class Image : public Texture {
public:
    Image(std::string name, std::vector<int> image, unsigned width, unsigned height);
    Color uv(double u, double v) const override;

    std::vector<int> image;
    unsigned width, height;
};