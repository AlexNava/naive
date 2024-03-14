cmake_minimum_required(VERSION 3.5)
project(Naive LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

include_directories(.)
include_directories(3d)

set(NAIVE_SOURCES
    
    naive_defs.cpp
    video.cpp
    screen.cpp
    viewport.cpp
    texture.cpp
    textureloader.cpp
    palette.cpp

    3d/renderer.cpp
    3d/obj3d.cpp
    3d/scene.cpp

    naive_defs.hpp
    video.hpp
    screen.hpp
    viewport.hpp
    texture.hpp
    textureloader.hpp
    palette.hpp

    3d/renderer.hpp
    3d/obj3d.hpp
    3d/scene.hpp
)

add_executable(Naive
    main.cpp
    ${NAIVE_SOURCES}
)

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES) # fixes bug with trailing whitespace in some SDL2 versions
target_link_libraries(Naive glm::glm)
target_link_libraries(Naive ${SDL2_LIBRARIES})
target_link_libraries(Naive SDL2_image::SDL2_image)

file(COPY "data" DESTINATION ${CMAKE_BINARY_DIR})

# Copy SDL2.dll and SDL2_image.dll to the output directory
file(COPY ${SDL2_DLL} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${SDL2_IMAGE_DLL} DESTINATION ${CMAKE_BINARY_DIR})
