#pragma once

#include "color.h"
#include <string>
#include <vector>
#include <algorithm>

class Pixels {
public:
  Pixels(int columns, int rows);

  const Color& operator()(int row, int col) const; // get pixel value
  Color& operator()(int row, int col); // set pixel value
  
  void save_ppm(const std::string& filename);
  void save_png(const std::string& filename);
  
  const int columns, rows;
  std::vector<Color> values;
};

double gamma_correction(double value);

template <class T>
T to_color(double value) {
  double clamped = std::clamp(value, 0.0, 0.999); // clamp value in range [0, 1)
  clamped = gamma_correction(clamped);
  return static_cast<T>(256*clamped); // scale to 8-bit color value
}
