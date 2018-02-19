#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>
#include <string>


class DisplayManager
{
private:
    SDL_Window *window;
    SDL_Surface* windowSurface;
    SDL_Renderer* renderer;
    const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Surface *loadSurface(const std::string &path);
	bool loadMedia();
public:
    DisplayManager(const std::string &title, int width, int height, uint32_t flags);
    ~DisplayManager();
    bool isInitialized() const;
    SDL_Window *operator()() const;
};

