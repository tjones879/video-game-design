cmake_minimum_required(VERSION 3.2)

project(VGD)
set(CMAKE_CXX_FLAGS "-std=c++14")

include_directories(${CMAKE_SOURCE_DIR}/lib/sdl-gpu/include)

# We must create a custom target for the sdl-gpu library because
# CMake fails to recognize the library as a required dependency
# during library linkage
add_custom_target(sdl-gpu ALL
    COMMAND make
    WORKING_DIRECTORY ./lib/sdl-gpu)

find_package(SDL2 REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/)
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/sdl-gpu)

add_executable(VGD
    src/main.cpp
    src/displaymanager.cpp
    src/eventhandler.cpp
    src/controller.cpp)
target_link_libraries(VGD ${SDL2_LIBRARIES} ${CMAKE_SOURCE_DIR}/build/lib/libSDL2_gpu.so)
add_dependencies(VGD sdl-gpu)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
