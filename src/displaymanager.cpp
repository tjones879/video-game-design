#include <inc/displaymanager.hpp>
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

#define DEBUG(e) std::cerr << e << std::endl;

DisplayManager::DisplayManager(const std::string &title)
    : window(NULL), initialized(false), gpu(&window){
	
    if (!gpu.screen) {
        std::cout << "sdl_gpu failed to initialize." << std::endl;
        return;
    } else {
        SDL_SetWindowTitle(window, title.c_str());
        initialized = true;
    }
    if( TTF_Init() == -1 ){
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
	}
}

DisplayManager::~DisplayManager()
{
    SDL_DestroyWindow(window);
    window = NULL;
}

DisplayManager::operator SDL_Window*() const
{
    return window;
}

bool DisplayManager::isInitialized() const
{
    return initialized;
}

void DisplayManager::displayCircle() {
    GPU_Clear(gpu);
    SDL_Color color;
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
