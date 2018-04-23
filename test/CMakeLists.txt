include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(testExe
    aabb.cpp
    collisions.cpp
    threadmanager.cpp
    vec2.cpp)

target_link_libraries(testExe engine gtest gtest_main)
add_test(
    NAME tests
    COMMAND testExe)
