#include <inc/dot.hpp>
#include <SDL2/SDL.h>

Dot::Dot() : posX(0), posY(0), velX(0), velY(0) {}

void Dot::handleEvent(SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && !e.key.repeat)
        switch (e.key.keysym.sym) {
        case SDLK_UP:
            velY += DOT_VEL;
            break;
        case SDLK_DOWN:
            velY -= DOT_VEL;
            break;
        case SDLK_LEFT:
            velX += DOT_VEL;
            break;
        case SDLK_RIGHT:
            velX -= DOT_VEL;
            break;
        }
}

void Dot::move()
{
    posX += velX;
    if ((posX < 0) || (posX + DOT_WIDTH > SCREEN_WIDTH))
        posX -= velX;
}

void Dot::render()
{

}
