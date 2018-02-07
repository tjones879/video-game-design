#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>
#include <string>


class Window
{
private:
    SDL_Window *window;
public:
    Window(const std::string &title, int width, int height, uint32_t flags);
    ~Window();
    bool isInitialized() const;
    SDL_Window *operator()() const;
};
