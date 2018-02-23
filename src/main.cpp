#include <SDL2/SDL.h>
#include <iostream>
#include "inc/eventhandler.hpp"
#include "inc/displaymanager.hpp"

constexpr int MIN_MILLISECONDS_PER_FRAME = 16;

int main(int argc, char **args)
{
    EventHandler eventHandler;
    if (!eventHandler.isInitialized()) {
        std::cout << "EventHandler failed" << std::endl;
        return 1;
    }
	
	DisplayManager displayManager("Test Window");
    if (!displayManager.isInitialized()) {
        std::cout << "Display Manager failed" << std::endl;
        return 1;
    }

    bool quit = false;
    SDL_Event e;
	SDL_Rect fillRect = { 640 / 4, 480 / 4, 640 / 2, 480 / 2 };
	while (!quit) {
		const int start = (int)SDL_GetTicks();
		displayManager.clearScreen();
		displayManager.renderRect(&fillRect,125,125,125,240);
		if(eventHandler.inputHandler(e) == 1) return 0;
		eventHandler.executeEvents();
		displayManager.renderScreen();
		const int end = (int)SDL_GetTicks();
		const int millisecondsThisFrame = end - start;
		if (millisecondsThisFrame < MIN_MILLISECONDS_PER_FRAME)
		{
			// If rendering faster than 60FPS, delay
			SDL_Delay(MIN_MILLISECONDS_PER_FRAME - millisecondsThisFrame);
		}

	}

    return 0;
}
