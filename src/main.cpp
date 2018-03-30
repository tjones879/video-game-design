#include <SDL2/SDL.h>
#include <iostream>
#include "inc/vec2.hpp"
#include "inc/threadmanager.hpp"
#include "inc/eventhandler.hpp"
#include "inc/displaymanager.hpp"
#include "inc/physics/world.hpp"
#include "inc/physics/common.hpp"
#include "inc/physics/polygon.hpp"
#include <chrono>

const std::chrono::milliseconds timePerFrame(16);

void displayThings(std::atomic<bool> *quit, ThreadManager *manager)
{
    DisplayManager displayManager("Test Window");
    const std::string renderBuff("render");

    if (!displayManager.isInitialized()) {
        std::cout << "Display Manager failed" << std::endl;
        return;
    }
    manager->openBuffer(renderBuff);

    while (!(*quit)) {
        auto start = std::chrono::high_resolution_clock::now();
        while (manager->newMessages(renderBuff)) {
            std::unique_ptr<RenderMessage> msg = manager->getMessage<RenderMessage>(renderBuff);
            displayManager.addRenderable(std::move(msg));
        }

        //displayManager.renderAll();
        auto end = std::chrono::high_resolution_clock::now();
        if (end - start < timePerFrame)
            std::this_thread::sleep_for(timePerFrame - (end - start));
    }
}

void makeSoundThings(std::atomic<bool> *quit, ThreadManager *manager)
{
}

int main(int argc, char **args)
{
    ThreadManager threadManager;

    EventHandler eventHandler;
    if (!eventHandler.isInitialized()) {
        std::cout << "EventHandler failed" << std::endl;
        return 1;
    }

    phy::World world(Vec2<float>(5, 9.8));
    phy::BodySpec spec;
    spec.bodyType = phy::BodyType::dynamicBody;
    auto shape = phy::PolygonShape(0.5f);
    shape.setBox(Vec2<float>(25, 25));
    auto body = world.createBody(spec);
    auto shape_ptr = body.lock()->addShape(shape);

    bool quit = false;
    SDL_Event e{};
    while (!quit) {
        auto start = std::chrono::high_resolution_clock::now();
        if (eventHandler.inputHandler(e) == 1)
            return 0;
        eventHandler.executeEvents();

        world.step();

        auto dt = std::chrono::high_resolution_clock::now() - start;
        if (dt < timePerFrame)
            std::this_thread::sleep_for(timePerFrame - dt);
    }
    return 0;
}
