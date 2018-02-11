#include <SDL2/SDL.h>
#include "inc/initialize.hpp"
#include "inc/window.hpp"
#include "inc/eventhandler.hpp"
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface *gCurrentSurface = NULL;

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

    /*
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("press.bmp");
    if (!gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT])
        std::cout << "Failed to load default image" << std::endl;
    */
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
    
    EventHandler eventhandler();
    if (!eventhandler.isInitialized()) {
        std::cout << "EventHandler failed" << std::endl;
        return 1;
    }

    gScreenSurface = SDL_GetWindowSurface(window());
    bool quit = false;
    SDL_Event e;

    if (loadMedia()) {
        while (!quit) {
			eventhandler.InputHandler(e);
            SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
            SDL_UpdateWindowSurface(window());
            SDL_Delay(16);

        }

    }

    return 0;
}
