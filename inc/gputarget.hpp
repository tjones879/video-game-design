#pragma once

#include <iostream>
#include "SDL2/SDL.h"
#include "SDL_gpu.h"

class GPUTarget {
public:
    GPU_Target *screen;
    /**
     * Initialize the sdl_gpu library and save the SDL_Window.
     */
    GPUTarget(SDL_Window **window);
    ~GPUTarget();
    SDL_Window *getWindow();
    operator GPU_Target *();
};
