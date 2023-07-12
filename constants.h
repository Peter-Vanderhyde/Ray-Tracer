#pragma once

#define _USE_MATH_DEFINES
#include <limits>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Constants {
  constexpr double Pi = M_PI;
  constexpr double Infinity = std::numeric_limits<double>::infinity();
  constexpr double Epsilon = 1e-6;
}

