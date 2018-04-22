set(SRC ${CMAKE_SOURCE_DIR}/src)

include_directories(${CMAKE_SOURCE_DIR}/../lib/sdl-gpu/include)

add_library(engine
    ${SRC}/physics/world.cpp
    ${SRC}/physics/body.cpp
    ${SRC}/physics/shape.cpp
    ${SRC}/physics/circle.cpp
    ${SRC}/physics/edge.cpp
    ${SRC}/physics/polygon.cpp
    ${SRC}/physics/aabb.cpp
    ${SRC}/physics/broadphase.cpp
    ${SRC}/physics/collisions.cpp
    ${SRC}/displaymanager.cpp
    ${SRC}/gputarget.cpp
    ${SRC}/eventhandler.cpp
    ${SRC}/controller.cpp)
