#pragma once

#include <SDL2/SDL.h>
#include "SDL_gpu.h"
#include <stdint.h>
#include <string>
#include "inc/gputarget.hpp"

#define DEBUG(e) std::cerr << e << std::endl;

class DisplayManager
{
private:
    SDL_Window *window;
    GPUTarget gpu;
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    bool initialized;
public:
    DisplayManager(const std::string &title);
    ~DisplayManager();
    bool isInitialized() const;
    operator SDL_Window*() const;
    void displayCircle();
};
