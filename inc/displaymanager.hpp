#pragma once

#include <SDL2/SDL.h>
#include "SDL_gpu.h"
#include <stdint.h>
#include <string>
//#include "SDL2/SDL_ttf.h"
#include "inc/gputarget.hpp"

#define DEBUG(e) std::cerr << e << std::endl;

class DisplayManager
{
private:
    bool initialized;
    SDL_Window *window;
    GPUTarget gpu;
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    bool initialized;
//    TTF_Font *font;
public:
    DisplayManager(const std::string &title);
    ~DisplayManager();
    bool isInitialized() const;
    operator SDL_Window*() const;
    void displayPolygon();
};
