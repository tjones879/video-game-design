#include <displaymanager.hpp>
#include <iostream>

DisplayManager::DisplayManager(const std::string &title, int width, int height, Uint32 flags)
    : window(NULL)
{
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width,
                              height, flags);
    if (!window)
        std::cout << "SDL Window Error: " << SDL_GetError() << std::endl;
}

DisplayManager::~DisplayManager()
{
    SDL_DestroyWindow(window);
}

bool DisplayManager::isInitialized() const
{
    return window;
}

SDL_Window *DisplayManager::operator()() const
{
    return window;
}

