#include "inc/gputarget.hpp"

GPUTarget::GPUTarget(SDL_Window **window)
{
    GPU_SetDebugLevel(GPU_DEBUG_LEVEL_MAX);
    screen = GPU_Init(640, 480, GPU_DEFAULT_INIT_FLAGS);
    *window = SDL_GetWindowFromID(screen->context->windowID);
}

GPUTarget::~GPUTarget()
{
    GPU_Quit();
}

SDL_Window *GPUTarget::getWindow()
{
    SDL_Window *window = SDL_GetWindowFromID(screen->context->windowID);
    return window;
}

GPUTarget::operator GPU_Target *()
{
    return screen;
}
