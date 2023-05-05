#include "texture.h"
#include "pixels.h"
#include "point2d.h"
#include <cmath>

Texture::Texture(std::string name, const Color& color)
    :name{name}, color{color} {}


Solid::Solid(std::string name, Color color)
    :Texture{name, color} {}

Color Solid::uv(double, double) const {
    return color;
}

Gradient::Gradient(std::string name, Color color)
    :Texture{name, color} {}

Color Gradient::uv(double u, double v) const {
    double percent_around = fabs(u - 0.5) / 0.5;
    return Color{1 - percent_around, 1 - v, 1 - percent_around * v};
}

Dots::Dots(std::string name, Color color)
    :Texture{name, color} {}

Color Dots::uv(double u, double v) const {
    return color * Color{fabs(fmod(u, 0.1) / 0.1 - 0.5) / 0.5 * fabs(fmod(v, 0.1) / 0.1 - 0.5) / 0.5, 1, 1};
}

Swirl::Swirl(std::string name, Color color, Color secondary, double num_of_stripes, double width_percent)
    :Texture{name, color}, secondary{secondary}, num_of_stripes{num_of_stripes}, width_percent{width_percent} {}

Color Swirl::uv(double u, double v) const {
    double stripe_width_percent = width_percent;
    double dif = fmod(u * v, 1 / num_of_stripes) / (1 / num_of_stripes);
    if (dif < 0.5 - (stripe_width_percent / 2) || dif > 0.5 + (stripe_width_percent / 2)) {
        return color;
    }
    else {
        dif = (dif - (0.5 - stripe_width_percent / 2)) / stripe_width_percent;
        dif = fabs(dif - 0.5) * 2.0;
        Color between = color - secondary;
        return color - between * (1 - dif);
    }
}

Squares::Squares(std::string name, Color color, Color secondary)
    :Texture{name, color}, secondary{secondary} {}

Color Squares::uv(double u, double v) const {
    if ((int)(u / 0.05) % 2 == 0 && (int)(v / 0.05) % 2 == 0) {
        return color;
    }
    else {
        return secondary;
    }
}

Checkered::Checkered(std::string name, Color color, Color secondary)
    :Texture{name, color}, secondary{secondary} {}

Color Checkered::uv(double u, double v) const {
    if ((int)(u / 0.1) % 2 == 1 && (int)(v / 0.1) % 2 == 0) {
        return color;
    }
    else if ((int)(u / 0.1) % 2 == 0 && (int)(v / 0.1) % 2 == 1) {
        return color;
    }
    else {
        return secondary;
    }
}

Image::Image(std::string name, std::vector<int> image, unsigned width, unsigned height)
    :Texture{name, {1, 1, 1}}, image{image}, width{width}, height{height} {}

Color Image::uv(double u, double v) const  {
    if (u < 0.0 || v < 0.0 || u > 1.0 || v > 1.0) {
        return {0, 0, 0};
    }
    int x = static_cast<int>(u * width);
    int y = static_cast<int>(v * height);
    // std::cout << v << '\n';
    int index = y * width + x;
    Color c{image[index * 4], image[index * 4 + 1], image[index * 4 + 2]};
    return c / 255.0;
}