#pragma once

#define _USE_MATH_DEFINES
#include <limits>
#include <cmath>

namespace Constants {
  constexpr double Pi = M_PI;
  constexpr double Infinity = std::numeric_limits<double>::infinity();
  constexpr double Epsilon = 1e-6;
}

