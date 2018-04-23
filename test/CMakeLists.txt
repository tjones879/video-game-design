include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(testExe
    aabb.cpp
    collisions.cpp
    threadmanager.cpp
    vec2.cpp
    world.cpp)

target_link_libraries(testExe engine gtest gtest_main ${SDL2_LIBRARIES})
add_test(
    NAME tests
    COMMAND testExe)
