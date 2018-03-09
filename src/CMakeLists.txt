set(SRC ${CMAKE_SOURCE_DIR}/src)

add_library(engine
    ${SRC}/physics/world.cpp
    ${SRC}/physics/body.cpp
    ${SRC}/physics/shape.cpp
    ${SRC}/physics/circle.cpp
    ${SRC}/physics/edge.cpp
    ${SRC}/physics/polygon.cpp
    ${SRC}/displaymanager.cpp
    ${SRC}/eventhandler.cpp
    ${SRC}/controller.cpp)
