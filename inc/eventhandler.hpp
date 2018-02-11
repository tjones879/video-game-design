#pragma once

#include <SDL2/SDL.h>


class EventHandler
{
private:
    bool initialized;
public:
    EventHandler();
    ~EventHandler();
    bool isInitialized() const;
    void InputHandler(SDL_Event &event);
};
