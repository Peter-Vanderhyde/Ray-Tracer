cmake_minimum_required(VERSION 3.16)
project(raytracer)

add_executable(main
  final.cpp
  color.cpp
  constants.cpp
  lodepng.cpp
  pixels.cpp
  ray.cpp
  sphere.cpp
  vector3d.cpp
  hit.cpp
  camera.cpp
  world.cpp
  parser.cpp
  random.cpp
  material.cpp
  save_details.cpp
  texture.cpp
  shape.cpp
  triangle.cpp
  vector2d.cpp
  fogbox.cpp
  fogsphere.cpp
  triangleobj.cpp
)
target_compile_options(main PUBLIC -Wall -Wextra -Ofast -pedantic -ggdb -fno-omit-frame-pointer -std=c++17)
target_link_libraries(main ${SOURCES})
