#include <SDL2/SDL.h>
#include <iostream>
#include "inc/vec2.hpp"
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

    phy::World world(Vec2<float>(0, 0));
    phy::BodySpec spec;
    spec.bodyType = phy::BodyType::dynamicBody;
    auto shape = phy::PolygonShape(0.5f);
    shape.setBox(Vec2<float>(25, 25));
    auto body = world.createBody(spec);
    auto shape_ptr = body.lock()->addShape(shape);

    auto shape2 = phy::PolygonShape(0.5f);
    shape2.setBox(Vec2<float>(50.0,50.0));
    auto body2 = world.createBody(spec);
    auto shape_ptr2 = body2.lock()->addShape(shape2);    

    bool quit = false;
    SDL_Event e{};
    while (!quit) {
        const int start = SDL_GetTicks();
        //displayManager.displayPolygon(body, shape_ptr);
        displayManager.displayPolygon(body2, shape_ptr2);
        if (eventHandler.inputHandler(e) == 1)
            return 0;
        eventHandler.executeEvents();

        world.step();
        //std::cout << *body.lock();

        const int millisecondsThisFrame = SDL_GetTicks() - start;
        if (millisecondsThisFrame < MIN_MILLISECONDS_PER_FRAME) {
            // If rendering faster than 60FPS, delay
            SDL_Delay(MIN_MILLISECONDS_PER_FRAME - millisecondsThisFrame);
        }
    }
    return 0;
}
