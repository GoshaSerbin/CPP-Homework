cmake_minimum_required(VERSION 3.0.0)
project(hm1 VERSION 0.1.0)

add_subdirectory(lib/utils)

add_executable(hm1 src/main.cpp)
target_link_libraries(hm1 utils)