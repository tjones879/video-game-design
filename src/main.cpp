#include <SDL2/SDL.h>
#include <iostream>
#include "inc/initialize.hpp"
#include "inc/window.hpp"
#include "inc/eventhandler.hpp"
#include "inc/displaymanager.hpp"


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
constexpr int MIN_MILLISECONDS_PER_FRAME = 16;

SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;

SDL_Surface *loadSurface(const std::string &path)
{
    SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());

    if (!loadedSurface) {
        std::cout << "Unable to load image: " << path.c_str() << SDL_GetError() << std::endl;
    }

    return loadedSurface;
}

bool loadMedia()
{
    bool success = true;
    gHelloWorld = SDL_LoadBMP("../assets/hello_world.bmp");
    if (!gHelloWorld) {
        std::cout << "Unable to load image: " << SDL_GetError() << std::endl;
        success = false;
    }

    return success;
}

int main(int argc, char **args)
{
    Initialize init(SDL_INIT_VIDEO);
    if (!init.isInitialized()) {
        std::cout << "Initialization failed" << std::endl;
        return 1;
    }

    Window window("Example Window", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window.isInitialized()) {
        std::cout << "Window failed" << std::endl;
        return 1;
    }
    
    EventHandler eventHandler;
    if (!eventHandler.isInitialized()) {
        std::cout << "EventHandler failed" << std::endl;
        return 1;
    }

    gScreenSurface = SDL_GetWindowSurface(window());
    bool quit = false;
    SDL_Event e;

    if (loadMedia()) {
        while (!quit) {
			const int start = (int)SDL_GetTicks();
			if(eventHandler.inputHandler(e) == 1) return 0;
			eventHandler.executeEvents();
            SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
            SDL_UpdateWindowSurface(window());
            const int end = (int)SDL_GetTicks();
			const int millisecondsThisFrame = end - start;
			if (millisecondsThisFrame < MIN_MILLISECONDS_PER_FRAME)
			{
				// If rendering faster than 60FPS, delay
				SDL_Delay(MIN_MILLISECONDS_PER_FRAME - millisecondsThisFrame);
			}

        }

    }

    return 0;
}
