#include <inc/displaymanager.hpp>
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <cstdio>

#define DEBUG(e) std::cerr << e << std::endl;

DisplayManager::DisplayManager(const std::string &title)
    : initialized(false), window(nullptr), gpu(&window)
{
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return;
    }
    if(TTF_Init()){
		std::cout<< "SDL_ttf could not initialize! SDL_ttf Error: %s\n"<<TTF_GetError()<<std::endl;
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

void DisplayManager::displayPolygon() {
    GPU_Clear(gpu);
    SDL_Color color{};
    color.r = 0;
    color.g = 0;
    color.b = 255;
    color.a = 255;
    float vertices[12] = {
        300,150,
        225,280,
        75,280,
        0,150,
        75,20,
        225,20,
    };
    GPU_PolygonFilled(gpu, 6, vertices, color);
    GPU_Flip(gpu);
}
