#include <SDL2/SDL.h>
#include <iostream>
#include "inc/eventhandler.hpp"
#include "inc/displaymanager.hpp"

const int MIN_MILLISECONDS_PER_FRAME = 16;

int main(int argc, char **args)
{
    DisplayManager displayManager("Test Window");
    if (!displayManager.isInitialized()) {
        std::cout << "Display Manager failed" << std::endl;
        return 1;
    }

    EventHandler eventHandler;
    if (!eventHandler.isInitialized()) {
        std::cout << "EventHandler failed" << std::endl;
        return 1;
    }

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        const int start = SDL_GetTicks();
        displayManager.displayPolygon();
        if (eventHandler.inputHandler(e) == 1)
            return 0;
        eventHandler.executeEvents();
        const int end = SDL_GetTicks();
        const int millisecondsThisFrame = end - start;
        if (millisecondsThisFrame < MIN_MILLISECONDS_PER_FRAME) {
            // If rendering faster than 60FPS, delay
            SDL_Delay(MIN_MILLISECONDS_PER_FRAME - millisecondsThisFrame);
        }
    }
    return 0;
}
