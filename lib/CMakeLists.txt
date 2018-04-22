file(GLOB nanovg_src nanovg/src/*.c)

add_library(
    nanovg_lib
    SHARED
    ${nanovg_src})
