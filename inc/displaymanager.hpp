#pragma once

#include <SDL2/SDL.h>
#include "SDL_gpu.h"
#include <memory>
#include <stdint.h>
#include <string>
#include "inc/physics/body.hpp"
#include "inc/gputarget.hpp"
#include "inc/physics/polygon.hpp"
#include "inc/messagetypes.hpp"

#define DEBUG(e) std::cerr << e << std::endl;

class DisplayManager
{
private:
    std::vector<std::pair<phy::PolygonShape, phy::Transform>> shapes;
    bool initialized;
    SDL_Window *window;
    GPUTarget gpu;
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    GPU_Camera camera;
    SDL_Color color{};
    SDL_Color playerColor{};
    bool r = false, g = false, b = false;
    void setCamera(Vec2<float> playerVel, Vec2<float> playerPos, Vec2<float> enemyVel, Vec2<float> enemyPos);
public:
    DisplayManager(const std::string &title);
    ~DisplayManager();
    bool isInitialized() const;
    operator SDL_Window*() const;
    void setPlayerColor();
    void displayAll();
    inline std::vector<float> toFloatVector(const phy::PolygonShape &shape, const phy::Transform &offset);
    void addRenderable(std::unique_ptr<RenderMessage>&& msg);
    int getCamPosX();
};
