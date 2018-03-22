#include <inc/displaymanager.hpp>
#include <iostream>
#include <cstdio>

#define DEBUG(e) std::cerr << e << std::endl;

DisplayManager::DisplayManager(const std::string &title)
    : initialized(false), window(nullptr), gpu(&window)
{
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return;
    }

    if (!gpu.screen) {
        std::cout << "sdl_gpu failed to initialize." << std::endl;
        return;
    } else {
        SDL_SetWindowTitle(window, title.c_str());
        initialized = true;
    }
}

DisplayManager::~DisplayManager()
{
    SDL_DestroyWindow(window);
    window = nullptr;
}

DisplayManager::operator SDL_Window*() const
{
    return window;
}

bool DisplayManager::isInitialized() const
{
    return initialized;
}

void DisplayManager::displayPolygon(const std::vector<std::weak_ptr<phy::Body>> &bodies,
                                    const std::vector<std::weak_ptr<phy::PolygonShape>> &shapes)
{
    GPU_Clear(gpu);
    SDL_Color color{};
    color.r = 0;
    color.g = 0;
    color.b = 255;
    color.a = 255;
    for (int i = 0; i < bodies.size(); i++) {
        std::weak_ptr<phy::Body> body = bodies[i];
        std::weak_ptr<phy::PolygonShape> shape = shapes[i];
        auto offset = body.lock()->getPosition();

        std::vector<float> vertices;
        vertices.reserve(shape.lock()->vertices.size() * 2);
        for (auto v : shape.lock()->vertices) {
            vertices.push_back(v.x + offset.x);
            vertices.push_back(v.y + offset.y);
        }
        GPU_PolygonFilled(gpu, shape.lock()->vertices.size(), &vertices[0], color);
    }
    GPU_Flip(gpu);
}
