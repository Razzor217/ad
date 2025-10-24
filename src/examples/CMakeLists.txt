add_executable(demo demo.cpp)
target_link_libraries(demo PRIVATE ad)
add_executable(linear_regression linear_regression.cpp)
target_link_libraries(linear_regression PRIVATE ad)
