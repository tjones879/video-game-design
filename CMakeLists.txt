cmake_minimum_required(VERSION 3.2)
project(VGD)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
# Make sure that SDL2 is installed on the machine.
find_package(SDL2 REQUIRED)
find_package(SDL2_Mixer REQUIRED)

# Attempt to compile the sdl-gpu library
# Do not compile components that we do not need
set (SDL_gpu_BUILD_DEMOS OFF)
set (SDL_gpu_BUILD_STATIC OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/sdl-gpu)

# We must create a custom target for the sdl-gpu library because
# CMake fails to recognize the library as a required dependency
# during library linkage
add_custom_target(sdl-gpu ALL
    COMMAND make
    WORKING_DIRECTORY ./lib/sdl-gpu)

# Include Google Test for testing
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/gtest)

# We add the compiler flags after including the sdl-gpu to avoid
# warnings from compiling that.
set(CMAKE_CXX_FLAGS "--std=c++14 -g -Wall \
         -Wno-zero-as-null-pointer-constant \
         -Wno-padded -Wno-unused-macros")

# Define all of our include paths
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/)
include_directories(${SDL_MIXER_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/lib/sdl-gpu/include)

enable_testing()

# Compile our subprojects
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)

# MISC Targets (clang-tidy)
file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE ALL_HEADER_FILES inc/*.hpp inc/*.h)

# Add command for static analysis if clang-tidy is installed
find_program(TIDY_PATH clang-tidy)
if(TIDY_PATH)
    add_custom_target(tidy COMMAND ${TIDY_PATH} ${ALL_SOURCE_FILES} -p ./)
endif()

# Create the actual game executable (./VGD) and link dependencies
add_executable(VGD src/main.cpp)
target_link_libraries(VGD
    ${SDL2_LIBRARIES}
    ${SDL_MIXER_LIBRARIES}
    engine
    ${CMAKE_SOURCE_DIR}/build/lib/libSDL2_gpu.so)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
