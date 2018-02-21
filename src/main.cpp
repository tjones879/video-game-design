#include <SDL2/SDL.h>
#include <iostream>
#include "inc/initialize.hpp"
#include "inc/window.hpp"
#include "inc/eventhandler.hpp"
#include "inc/displaymanager.hpp"


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int JOYSTICK_DEAD_ZONE = 8000;
constexpr int MIN_MILLISECONDS_PER_FRAME = 16;

SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;
SDL_Joystick *gGameController = NULL;


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
    Initialize init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
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
        auto numJoysticks = SDL_NumJoysticks();
        if (numJoysticks < 1) {
            std::cout << "No joysticks" << std::endl;
        } else {
            std::cout << numJoysticks << " joysticks found:" << std::endl;
            for (int i = 0; i < numJoysticks; i++) {
                SDL_Joystick *js = SDL_JoystickOpen(i);
                std::cout << "Name: " << SDL_JoystickName(js) << std::endl
                          << "    NumAxes: " << SDL_JoystickNumAxes(js) << std::endl
                          << "    NumBalls: " << SDL_JoystickNumBalls(js) << std::endl
                          << "    NumButtons: " << SDL_JoystickNumButtons(js) << std::endl
                          << "    NumHats: " << SDL_JoystickNumHats(js) << std::endl;
            }
        }
        while (!quit) {
            const int start = (int)SDL_GetTicks();
            if(eventHandler.inputHandler(e) == 1)
                return 0;
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
