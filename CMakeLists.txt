cmake_minimum_required(VERSION 3.16)

# Automatically detect the correct GCC and G++ compilers from MSYS2 environment
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
endif()

# Project definition
project(raytracer)

# Compiler flags for optimization and debugging
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Ofast -pedantic -ggdb -fno-omit-frame-pointer -std=c++17 -static-libgcc -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

# Source files for the main executable
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

# Create a second executable for extract_details
add_executable(extract_details extract_details.cpp save_details.cpp lodepng.cpp)

# Apply the necessary compile options for extract_details
target_compile_options(extract_details PUBLIC -std=c++17)

# Linking the sources (if necessary, otherwise you can remove these)
target_link_libraries(extract_details ${SOURCES})