cmake_minimum_required(VERSION 3.2)
project(VGD)

include_directories(${CMAKE_SOURCE_DIR}/lib/sdl-gpu/include)

# We must create a custom target for the sdl-gpu library because
# CMake fails to recognize the library as a required dependency
# during library linkage
add_custom_target(sdl-gpu ALL
    COMMAND make
    WORKING_DIRECTORY ./lib/sdl-gpu)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "--std=c++14 -g -Wall \
         -Wno-zero-as-null-pointer-constant \
         -Wno-padded -Wno-unused-macros")


find_package(SDL2 REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/)
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/sdl-gpu)

# MISC Targets (clang-format)
file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE ALL_HEADER_FILES inc/*.hpp inc/*.h)

add_executable(VGD
    src/main.cpp
    src/physics/common.cpp
    src/physics/world.cpp
    src/physics/body.cpp
    src/physics/shape.cpp
    src/physics/circle.cpp
    src/physics/edge.cpp
    src/physics/polygon.cpp
    src/displaymanager.cpp
    src/eventhandler.cpp
    src/controller.cpp)
target_link_libraries(VGD ${SDL2_LIBRARIES} ${CMAKE_SOURCE_DIR}/build/lib/libSDL2_gpu.so)
add_dependencies(VGD sdl-gpu)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

find_program(TIDY_PATH clang-tidy)
if(TIDY_PATH)
    add_custom_target(tidy COMMAND ${TIDY_PATH} ${ALL_SOURCE_FILES} -p ./)
endif()
