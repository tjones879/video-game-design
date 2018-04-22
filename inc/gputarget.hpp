#pragma once

#include <iostream>
#include "SDL2/SDL.h"
#include "SDL_gpu.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg.h"
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"

#define GLEW_STATIC
#include <GL/glew.h>

class GPUTarget {
public:
    GPU_Target *screen;
    NVGcontext *vg;
    /**
     * Initialize the sdl_gpu library and save the SDL_Window.
     */
    GPUTarget(SDL_Window **window);
    ~GPUTarget();
    SDL_Window *getWindow();
    operator GPU_Target *();
    void createGUI(const uint16_t screen_w, const uint16_t screen_h);
};
