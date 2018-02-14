cmake_minimum_required(VERSION 3.2)

project(VGD)
set(CMAKE_CXX_FLAGS "-std=c++11")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/)

add_executable(VGD
    src/main.cpp
    src/window.cpp
    src/initialize.cpp
    src/eventhandler.cpp)
target_link_libraries(VGD ${SDL2_LIBRARIES})
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
