cmake_minimum_required(VERSION 3.0.0)
set(ProjectName hw1)
project(${ProjectName})

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EnableTesting ON)

if(${EnableTesting})
    add_subdirectory(tests)
endif()

add_subdirectory(lib/Application)

set(SOURCES src/main.cpp)
add_executable(${ProjectName} ${SOURCES})
target_link_libraries(${ProjectName} Application)
