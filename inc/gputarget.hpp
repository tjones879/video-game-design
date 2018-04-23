#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg.h"

#include <iostream>
#include "SDL2/SDL.h"
#include "SDL_gpu.h"

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
