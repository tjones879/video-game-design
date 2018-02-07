#include <inc/window.hpp>
#include <iostream>

Window::Window(const std::string &title, int width, int height, Uint32 flags)
    : window(NULL)
{
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width,
                              height, flags);
    if (!window)
        std::cout << "SDL Window Error: " << SDL_GetError() << std::endl;
}

Window::~Window()
{
    SDL_DestroyWindow(window);
}

bool Window::isInitialized() const
{
    return window;
}

SDL_Window *Window::operator()() const
{
    return window;
}
