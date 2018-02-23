#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>
#include <string>

#define DEBUG(e) std::cerr << e << std::endl;

class DisplayManager
{
private:
    SDL_Window *window;
    SDL_Surface* windowSurface;
    SDL_Renderer* renderer;
    const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	bool loadMedia();
	bool initialized;
	SDL_Surface* testSurface;
public:
    DisplayManager(const std::string &title);
    ~DisplayManager();
    bool isInitialized() const;
    SDL_Window *operator()() const;
    SDL_Renderer* getRenderer();
    void clearScreen();
    void renderScreen();
    void renderRect(const SDL_Rect* rect, const int red, const int blue, const int green, const int alpha);
};
