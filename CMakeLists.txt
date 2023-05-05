cmake_minimum_required(VERSION 3.16)
project(raytracer)

add_executable(main
  test-hit4.cpp
  color.cpp
  constants.cpp
  lodepng.cpp
  pixels.cpp
  ray.cpp
  sphere.cpp
  vector3d.cpp
  hit.cpp
)
target_compile_options(main PUBLIC -Wall -Wextra -pedantic -ggdb -std=c++17)
target_link_libraries(main ${SOURCES})
