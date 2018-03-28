#pragma once

#include <SDL2/SDL.h>
#include "SDL_gpu.h"
#include <memory>
#include <stdint.h>
#include <string>
#include "inc/physics/body.hpp"
#include "inc/gputarget.hpp"
#include "inc/physics/polygon.hpp"

#define DEBUG(e) std::cerr << e << std::endl;

class DisplayManager
{
private:
    bool initialized;
    SDL_Window *window;
    GPUTarget gpu;
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
public:
    DisplayManager(const std::string &title);
    ~DisplayManager();
    bool isInitialized() const;
    operator SDL_Window*() const;
    void displayPolygon(const std::vector<std::weak_ptr<phy::Body>> &bodies,
                        const std::vector<std::weak_ptr<phy::PolygonShape>> &shapes);
};
