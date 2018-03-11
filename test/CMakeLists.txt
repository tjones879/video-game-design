include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(testExe
    vec2.cpp
    aabb.cpp)
target_link_libraries(testExe engine gtest gtest_main)
add_test(
    NAME tests
    COMMAND testExe)
