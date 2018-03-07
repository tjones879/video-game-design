add_executable(testExe
    main.cpp)
target_link_libraries(testExe engine)
add_custom_target(tests ./testExe
    DEPENDS testExe)
