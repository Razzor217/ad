cmake_minimum_required(VERSION 3.10)

project(ad LANGUAGES CXX)
enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
set(CMAKE_CXX_FLAGS "-std=c++23 -Wall -Wextra -march=native -O0 -g")

add_subdirectory(src)
add_subdirectory(test)
