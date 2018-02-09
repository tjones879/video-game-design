#pragma once
#include <SDL2/SDL.h>

class Dot
{
public:
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;
    static const int DOT_VEL = 20;

    Dot();
    void handleEvent(SDL_Event &e);
    void move();
    void render();
private:
    int posX, posY;
    int velX, velY;
};
