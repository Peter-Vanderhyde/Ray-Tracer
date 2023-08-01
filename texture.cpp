#include "texture.h"
#include "pixels.h"
#include "point2d.h"
#include "material.h"
#include <cmath>

Texture::Texture(std::string name, const Color& color)
    :name{name}, color{color} {}

double Texture::opacity(double, double) const {
    // Default full opacity
    return 1.0;
}

Solid::Solid(Color color)
    :Texture{"solid", color} {}

Color Solid::uv(double, double) const {
    return color;
}

Gradient::Gradient(Color color, Color secondary)
    :Texture{"gradient", color}, secondary{secondary} {}

Color Gradient::uv(double, double v) const {
    // Define the two colors for the gradient
    Color color1 = color;  // Starting color (e.g., red)
    Color color2 = secondary;  // Ending color (e.g., blue)

    double percent_up = v;

    // Interpolate the colors based on the percentage
    Color interpolated_color = color1 * (1.0 - percent_up) + color2 * percent_up;

    return interpolated_color;
}

Dots::Dots(Color color)
    :Texture{"dots", color} {}

Color Dots::uv(double u, double v) const {
    return color * Color{fabs(fmod(u, 0.1) / 0.1 - 0.5) / 0.5 * fabs(fmod(v, 0.1) / 0.1 - 0.5) / 0.5, 1, 1};
}

Swirl::Swirl(Color color, Color secondary, double num_of_stripes, double width_percent)
    :Texture{"swirl", color}, secondary{secondary}, num_of_stripes{num_of_stripes}, width_percent{width_percent} {}

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

Squares::Squares(Color color, Color secondary)
    :Texture{"squares", color}, secondary{secondary} {}

Color Squares::uv(double u, double v) const {
    if ((int)(u / 0.05) % 2 == 0 && (int)(v / 0.05) % 2 == 0) {
        return color;
    }
    else {
        return secondary;
    }
}

Checkered::Checkered(Color color, Color secondary)
    :Texture{"checkered", color}, secondary{secondary} {}

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

Image::Image(std::vector<int> image, unsigned width, unsigned height)
    :Texture{"image", {1, 1, 1}}, image{image}, width{width}, height{height} {}

Color Image::uv(double u, double v) const  {
    if (u < 0.0 || v < 0.0 || u > 1.0 || v > 1.0) {
        return {0, 0, 0};
    }
    int x = static_cast<int>(u * width);
    int y = static_cast<int>(v * height);
    int index = y * width + x;
    double r, g, b;
    r = static_cast<double>(image[index * 4]);
    g = static_cast<double>(image[index * 4 + 1]);
    b = static_cast<double>(image[index * 4 + 2]);
    Color c{r, g, b};
    return c / 255.0;
}

double Image::opacity(double u, double v) const {
    if (u < 0.0 || v < 0.0 || u > 1.0 || v > 1.0) {
        return 0.0;
    }
    int x = static_cast<int>(u * width);
    int y = static_cast<int>(v * height);
    int index = y * width + x;
    double opacity = image[index * 4 + 3];
    return opacity / 255.0;
}