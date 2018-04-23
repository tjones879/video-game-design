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
const std::string renderBuff("render");
const std::string inputBuff("input");

void display(std::atomic<bool> *quit, ThreadManager *manager)
{
    DisplayManager displayManager("Test Window");

    if (!displayManager.isInitialized()) {
        std::cout << "Display Manager failed" << std::endl;
        return;
    }
    manager->openBuffer(renderBuff);

    while (!(*quit)) {
        auto start = std::chrono::high_resolution_clock::now();
        while (manager->newMessages(renderBuff)) {
            auto&& msg = manager->getMessage<RenderMessage>(renderBuff);
            displayManager.addRenderable(std::move(msg));
        }

        displayManager.displayAll();
        auto end = std::chrono::high_resolution_clock::now();
        if (end - start < timePerFrame)
            std::this_thread::sleep_for(timePerFrame - (end - start));
    }
}

void physics(std::atomic<bool> *quit, ThreadManager *manager)
{
    phy::World world(Vec2<float>(5, 9.8));

    manager->openBuffer(inputBuff);

    phy::BodySpec spec;
    spec.bodyType = phy::BodyType::dynamicBody;
    spec.position = {100, 100};
    spec.gravityFactor = 1;
    auto shape = phy::PolygonShape(1.0f);
    shape.setBox(Vec2<float>(25, 25));
    auto body = world.createBody(spec);
    auto shape_ptr = body.lock()->addShape(shape);

    while (!(*quit)) {
        auto start = std::chrono::high_resolution_clock::now();
        while (manager->newMessages(inputBuff)) {
            auto&& msg = manager->getMessage<InputMessage>(inputBuff);
        }

        world.step();
        manager->sendMessage(renderBuff, std::move(world.getObjects()));

        auto dt = std::chrono::high_resolution_clock::now() - start;
        if (dt < timePerFrame)
            std::this_thread::sleep_for(timePerFrame - dt);
    }
}

int main(int argc, char **args)
{
    ThreadManager threadManager;

    threadManager.spawnThread(display);
    threadManager.spawnThread(physics);

    EventHandler eventHandler;
    if (!eventHandler.isInitialized()) {
        std::cout << "EventHandler failed" << std::endl;
        return 1;
    }

    bool quit = false;
    SDL_Event e{};
    while (!quit) {
        auto start = std::chrono::high_resolution_clock::now();
        if (eventHandler.inputHandler(e) == 1) {
            threadManager.waitAll();
            return 0;
        }

        eventHandler.executeEvents();

        auto dt = std::chrono::high_resolution_clock::now() - start;
        if (dt < timePerFrame)
            std::this_thread::sleep_for(timePerFrame - dt);
    }
    return 0;
}
