cmake_minimum_required(VERSION 3.0.0)
set(ProjectName hw1)
project(${ProjectName} VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(CMAKE_EXPORT_COMPILE_COMMANDS True)

option(EnableTesting "Enable testing" True)

set(SOURCES src/main.cpp
    src/Application.cpp
    src/InputParser.cpp
    src/MusicDataBaseHandler.cpp
    src/OutputFormatter.cpp)
set(INCLUDES include)

if(EnableTesting)
    add_subdirectory(tests)
    add_compile_options(--coverage)
    add_link_options(--coverage)

    # add_compile_options(-fsanitize=address)
endif()

include_directories(${INCLUDES})
add_executable(${ProjectName} ${SOURCES})
