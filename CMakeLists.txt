cmake_minimum_required(VERSION 3.2)
project(VGD)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "--std=c++14 -g -Wall \
         -Wno-zero-as-null-pointer-constant \
         -Wno-padded -Wno-unused-macros")


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/)

# MISC Targets (clang-format)
file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE ALL_HEADER_FILES inc/*.hpp inc/*.h)

add_executable(VGD
    src/main.cpp
    src/window.cpp
    src/initialize.cpp
    src/physics/world.cpp
    src/physics/body.cpp
    src/physics/shape.cpp
    src/physics/circle.cpp
    src/physics/edge.cpp
    src/physics/polygon.cpp)
target_link_libraries(VGD ${SDL2_LIBRARIES})
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

find_program(TIDY_PATH clang-tidy)
if(TIDY_PATH)
    add_custom_target(tidy COMMAND ${TIDY_PATH} ${ALL_SOURCE_FILES} -p ./)
endif()
