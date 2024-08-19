cmake_minimum_required(VERSION 3.5)
project(Naive LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(NAIVE_SOURCES
    naive_defs.cpp
    events.cpp
    video.cpp
    screen.cpp
    viewport.cpp
    texture.cpp
    textureloader.cpp
    palette.cpp

    3d/renderer.cpp
    3d/scene.cpp
    3d/scenenode.cpp
    3d/objloader.cpp
    3d/mesh.cpp
)

set(NAIVE_HEADERS
    naive.hpp

    naive_defs.hpp
    events.hpp
    video.hpp
    screen.hpp
    viewport.hpp
    texture.hpp
    textureloader.hpp
    palette.hpp

    2d/screen_ops.hpp
    3d/renderer.hpp
    3d/scene.hpp
    3d/scenenode.hpp
    3d/objloader.hpp
    3d/obj3d.hpp
    3d/mesh.hpp
)

add_library(naive STATIC)

add_executable(naive_test
    main.cpp
)

find_package(glm REQUIRED)
target_include_directories(naive PRIVATE ${GLM_INCLUDE_DIRS})

find_package(SDL2 REQUIRED)
target_include_directories(naive PRIVATE ${SDL2_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)
target_include_directories(naive PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})

target_include_directories(naive PRIVATE .)
target_include_directories(naive PRIVATE 3d)
target_include_directories(naive PRIVATE libs)

target_sources(naive
    PRIVATE ${NAIVE_SOURCES}
    PUBLIC FILE_SET HEADERS # reserved name: target_sources File set TYPE may only be "HEADERS" or "CXX_MODULES"
        BASE_DIRS ${PROJECT_SOURCE_DIR}
        FILES ${NAIVE_HEADERS}
)

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES) # fixes bug with trailing whitespace in some SDL2 versions
target_link_libraries(naive glm::glm)
target_link_libraries(naive ${SDL2_LIBRARIES})
target_link_libraries(naive SDL2_image::SDL2_image)

file(COPY "data" DESTINATION ${CMAKE_BINARY_DIR})

# Copy SDL2.dll and SDL2_image.dll to the output directory
file(COPY ${SDL2_DLL} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${SDL2_IMAGE_DLL} DESTINATION ${CMAKE_BINARY_DIR})

# Copy header files to the output directory
#file(COPY)

target_include_directories(naive_test PRIVATE .)
target_link_libraries(naive_test naive)

# install typically puts files to a shared location, outside the project tree
install(TARGETS naive FILE_SET HEADERS)
