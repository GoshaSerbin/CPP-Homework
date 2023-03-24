cmake_minimum_required(VERSION 3.0.0)
set(ProjectName hw1)
project(${ProjectName})

set(CMAKE_CXX_STANDARD 17)

option(EnableTesting ON)

if(${EnableTesting})
    add_subdirectory(tests)
endif()

add_subdirectory(tests)
add_subdirectory(lib/utils)

set(SOURCES src/main.cpp)
add_executable(${ProjectName} ${SOURCES})
target_link_libraries(${ProjectName} utils)