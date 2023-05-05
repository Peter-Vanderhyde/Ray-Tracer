cmake_minimum_required(VERSION 3.16)
project(raytracer)

add_executable(test-pixels
  test-pixels.cpp
  pixels.cpp
  vector3d.cpp
  lodepng.cpp
)
target_compile_options(test-pixels PUBLIC -Wall -Wextra -pedantic -ggdb -std=c++17)
target_link_libraries(test-pixels ${SOURCES})
