cmake_minimum_required(VERSION 3.5)
project(Naive LANGUAGES CXX)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories("./glm")

set(NAIVE_SOURCES
    main.cpp
    
    naive_defs.cpp
    video.cpp
    texture.cpp
    palette.cpp

    naive_defs.hpp
    video.hpp
    texture.hpp
    palette.hpp
)

add_executable(Naive ${NAIVE_SOURCES})

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES) # fixes bug with trailing whitespace in some SDL2 versions
target_link_libraries(Naive ${SDL2_LIBRARIES})
