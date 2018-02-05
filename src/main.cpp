#include <SDL2/SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char **args)
{
    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
    } else {
        window = SDL_CreateWindow("Example Window", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == NULL) {
            std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
        } else {
            screenSurface = SDL_GetWindowSurface(window);
            SDL_FillRect(screenSurface, NULL,
                         SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
            SDL_UpdateWindowSurface(window);
            SDL_Delay(2000);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
