#include "pixels.h"
#include "color.h"
#include <cmath>


// write a function to set all the pixels to the same color
void set_color(Pixels& pixels, Color color) {
  for (int y = 0; y < pixels.rows; y++) {
    for (int x = 0; x < pixels.columns; x++) {
      pixels(y, x) = color;
    }
  }
}

// write a function that produces a single color gradient
void gradient(Pixels& pixels) {
  double whiteness;
  double max = std::max(pixels.rows, pixels.columns);
  double x_rate = max / pixels.columns;
  double y_rate = max / pixels.rows;
  double rate = 255.0 / max;
  for (int y = 0; y < pixels.rows; y++) {
    for (int x = 0; x < pixels.columns; x++) {
      whiteness = (255 - abs(x_rate * x - y_rate * y) * rate) / 255;
      pixels(y, x) = Color(1, whiteness, whiteness);
    }
  }
}

// write a function to color a pretty picture!
void pretty(Pixels& pixels) {
  double red, green, blue;
  double max = std::max(pixels.rows, pixels.columns);
  double x_rate = max / pixels.columns;
  double y_rate = max / pixels.rows;
  double rate = 255.0 / max;
  for (int y = 0; y < pixels.rows; y++) {
    for (int x = 0; x < pixels.columns; x++) {
      red = (255 - abs(x_rate * x - y_rate * y) * rate) / 255;
      green = (255 - abs(x_rate * (pixels.columns - x) - y_rate * y) * rate) / 255;
      double section = 255 - abs(x * x_rate - pixels.columns / 2) + sin(y * 0.005) * 40;
      if (section < 0 && x > pixels.columns / 2) {
        blue = 0.5;
        green = 255 - green;
      } else if (section > 0) {
        blue = sqrt(red);
        red *= 2;
        green /= 2;
      }
      pixels(y, x) = Color(red, green, blue);
    }
  }
}

void pretty2(Pixels& pixels) {
  double red, green, blue;
  for (int y = 0; y < pixels.rows; y++) {
    for (int x = 0; x < pixels.columns; x++) {
      double val = 0.005;
      red = cos((x - y) * val / 1.5);
      green = sin((x - y) * val / 2);
      blue = cos((x - y) * val / 4);
      pixels(y, x) = Color(red, green, blue);
    }
  }
}

int main() {
  const unsigned columns = 1280;
  const unsigned rows = 720;
  Pixels pixels{columns, rows};

  set_color(pixels, Magenta);
  std::string filename{"magenta.png"};
  pixels.save_png(filename);
  std::cout << "Saved magenta\n";

  gradient(pixels);
  filename = "gradient.png";
  pixels.save_png(filename);
  std::cout << "Saved gradient\n";

  pretty(pixels);
  filename = "pretty.png";
  pixels.save_png(filename);
  std::cout << "Saved pretty\n";

  pretty2(pixels);
  filename = "pretty2.png";
  pixels.save_png(filename);
  std::cout << "Saved pretty2\n";
  
  std::cout << "Done" << '\n';
}
