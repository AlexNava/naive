cmake_minimum_required(VERSION 3.5)
project(Naive LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

include_directories("./glm")

set(NAIVE_SOURCES
    main.cpp
    
    naive_defs.cpp
    video.cpp
    texture.cpp
    textureloader.cpp
    palette.cpp

    naive_defs.hpp
    video.hpp
    texture.hpp
    textureloader.hpp
    palette.hpp
)

add_executable(Naive ${NAIVE_SOURCES})

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES) # fixes bug with trailing whitespace in some SDL2 versions
target_link_libraries(Naive ${SDL2_LIBRARIES})
target_link_libraries(Naive SDL2_image::SDL2_image)

file(COPY "data" DESTINATION ${CMAKE_BINARY_DIR})

# Copy SDL2.dll and SDL2_image.dll to the output directory
file(COPY ${SDL2_DLL} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${SDL2_IMAGE_DLL} DESTINATION ${CMAKE_BINARY_DIR})
