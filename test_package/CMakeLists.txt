cmake_minimum_required(VERSION 3.10)
project(ad_test CXX)

find_package(ad CONFIG REQUIRED)

add_executable(example src/example.cpp)
target_link_libraries(example PRIVATE ad::ad)
