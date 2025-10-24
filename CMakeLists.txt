cmake_minimum_required(VERSION 3.10)

project(ad LANGUAGES CXX)
enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-std=c++23 -Wall -Wextra -march=native -O0 -g")

find_package(Eigen3 REQUIRED)

add_library(ad INTERFACE)
target_include_directories(ad INTERFACE include)
target_link_libraries(ad INTERFACE Eigen3::Eigen)

add_subdirectory(src)
add_subdirectory(test)
