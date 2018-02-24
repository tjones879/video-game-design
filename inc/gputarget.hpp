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
    GPUTarget(SDL_Window **window)
    {
        GPU_SetDebugLevel(GPU_DEBUG_LEVEL_MAX);
        screen = GPU_Init(640, 480, GPU_DEFAULT_INIT_FLAGS);
        *window = SDL_GetWindowFromID(screen->context->windowID);
    }

    ~GPUTarget()
    {
        GPU_Quit();
    }

    SDL_Window *getWindow()
    {
        SDL_Window *window = SDL_GetWindowFromID(screen->context->windowID);
        return window;
    }

    operator GPU_Target *()
    {
        return screen;
    }
};
