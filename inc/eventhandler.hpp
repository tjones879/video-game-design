#pragma once

#include <SDL2/SDL.h>
#include <array>
#include <iterator>


class EventHandler
{
private:
    bool initialized;
    std::array<bool,285> keystate{};
public:
    EventHandler();
    ~EventHandler();
    bool isInitialized() const;
    int InputHandler(SDL_Event &event);
};
