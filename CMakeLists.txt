cmake_minimum_required(VERSION 3.2)

project(VGD)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(VGD src/main.cpp)
target_link_libraries(VGD ${SDL2_LIBRARIES})
