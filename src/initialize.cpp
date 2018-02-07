#include <inc/initialize.hpp>
#include <iostream>

Initialize::Initialize(Uint32 flags) : initialized(false)
{
    if (SDL_Init(flags))
        std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;

    initialized = true;
}

Initialize::~Initialize()
{
    SDL_Quit();
}

bool Initialize::isInitialized() const
{
    return initialized;
}
