#include <SDL2/SDL.h>
#include "inc/initialize.hpp"
#include "inc/window.hpp"
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
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface *gCurrentSurface = NULL;
SDL_Renderer *gRenderer = NULL;

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

    gRenderer = SDL_CreateRenderer(window(), -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer) {
        std::cout << "Renderer could not be created!" << SDL_GetError() << std::endl;
        return 1;
    }

    gScreenSurface = SDL_GetWindowSurface(window());
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)
                quit = true;
        }
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(gRenderer, &fillRect);
        SDL_RenderPresent(gRenderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(gRenderer);
    return 0;
}
