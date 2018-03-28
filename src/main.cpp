#include <SDL2/SDL.h>
#include <iostream>
#include "inc/vec2.hpp"
#include "inc/eventhandler.hpp"
#include "inc/displaymanager.hpp"
#include "inc/physics/world.hpp"
#include "inc/physics/common.hpp"
#include "inc/physics/polygon.hpp"

#define DEBUG(e) std::cerr << e << std::endl;

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
    spec.position = {100, 100};
    spec.gravityFactor = 1;
    auto shape = phy::PolygonShape(1.0f);
    shape.setBox(Vec2<float>(25, 25));
    auto body = world.createBody(spec);
    auto shape_ptr = body.lock()->addShape(shape);

    spec.position = {150,150};
    auto shape2 = phy::PolygonShape(0.5f);
    shape2.setBox(Vec2<float>(50.0,50.0));
    auto body2 = world.createBody(spec);
    auto shape_ptr2 = body2.lock()->addShape(shape2);
    eventHandler.setPlayer(body2);
    std::vector<std::weak_ptr<phy::Body>> bodies;
    bodies.push_back(body);
    bodies.push_back(body2);
    std::vector<std::weak_ptr<phy::PolygonShape>> shapes;
    shapes.push_back(shape_ptr);
    shapes.push_back(shape_ptr2);

    bool quit = false;
    SDL_Event e{};
    while (!quit) {
        const int start = SDL_GetTicks();
        displayManager.displayPolygon(bodies, shapes);
        if (eventHandler.inputHandler(e) == 1)
            return 0;
        eventHandler.executeEvents();

        world.step();

        const int millisecondsThisFrame = SDL_GetTicks() - start;
        if (millisecondsThisFrame < MIN_MILLISECONDS_PER_FRAME) {
            // If rendering faster than 60FPS, delay
            SDL_Delay(MIN_MILLISECONDS_PER_FRAME - millisecondsThisFrame);
        }
    }
    return 0;
}
