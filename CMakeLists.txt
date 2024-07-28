cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe")
project(raytracer)

add_executable(main
  main.cpp
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
  normal.cpp
  sun.cpp
  trianglebillboard.cpp
  combine_images.cpp
  skysphere.cpp
  property_map.cpp
)
target_compile_options(main PUBLIC -Wall -Wextra -Ofast -pedantic -ggdb -fno-omit-frame-pointer -std=c++17)
target_link_libraries(main ${SOURCES})

add_executable(extract_details extract_details.cpp save_details.cpp lodepng.cpp)
target_compile_options(extract_details PUBLIC -std=c++17)
target_link_libraries(extract_details ${SOURCES})