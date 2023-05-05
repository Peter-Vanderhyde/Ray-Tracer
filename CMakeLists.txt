cmake_minimum_required(VERSION 3.20)
project(factorial)

# adjust your compiler settings
add_compile_options(-g -Wall -Wextra -std=c++17)

# create a library that will be used in other parts of our program
add_library(factorial factorial.cpp)

# main program executable to create
add_executable(main main.cpp)
target_link_libraries(main PUBLIC factorial)

# test program
add_executable(test test-factorial.cpp)
target_link_libraries(test PUBLIC factorial)
