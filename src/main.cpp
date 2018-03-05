#include <SDL2/SDL.h>
#include <iostream>
#include "inc/eventhandler.hpp"
#include "inc/displaymanager.hpp"
#include "inc/physics/world.hpp"
#include "inc/physics/common.hpp"
#include "inc/physics/polygon.hpp"

const int MIN_MILLISECONDS_PER_FRAME = 16;

int main(int argc, char **args)
{
    DisplayManager displayManager("Test Window");
    if (!displayManager.isInitialized()) {
        std::cout << "Display Manager failed" << std::endl;
        return 1;
    }

    EventHandler eventHandler;
    if (!eventHandler.isInitialized()) {
        std::cout << "EventHandler failed" << std::endl;
        return 1;
    }

    phy::World world(phy::Vec2(3, 5));
    phy::BodySpec spec;
    spec.bodyType = phy::BodyType::dynamicBody;
    auto shape = phy::PolygonShape(0.5f);
    shape.setBox(phy::Vec2(25, 25));
    auto body = world.createBody(spec);
    auto shape_ptr = body.lock()->addShape(shape);

    bool quit = false;
    SDL_Event e{};
    while (!quit) {
        const int start = SDL_GetTicks();
        displayManager.displayPolygon(body, shape_ptr);
        if (eventHandler.inputHandler(e) == 1)
            return 0;
        eventHandler.executeEvents();

        world.step();
        std::cout << *body.lock();

        const int millisecondsThisFrame = SDL_GetTicks() - start;
        if (millisecondsThisFrame < MIN_MILLISECONDS_PER_FRAME) {
            // If rendering faster than 60FPS, delay
            SDL_Delay(MIN_MILLISECONDS_PER_FRAME - millisecondsThisFrame);
        }
    }
    return 0;
}
