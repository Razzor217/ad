cmake_minimum_required(VERSION 3.10)

project(ad LANGUAGES CXX)
enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-std=c++23 -Wall -Wextra -march=native -O0 -g")

add_library(ad INTERFACE)
target_include_directories(ad INTERFACE include)

add_subdirectory(src)
add_subdirectory(test)
