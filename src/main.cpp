#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "inc/threadmanager.hpp"
#include <iostream>
#include "inc/vec2.hpp"
#include "inc/physics/world.hpp"
#include "inc/eventhandler.hpp"
#include "inc/displaymanager.hpp"
#include "inc/physics/common.hpp"
#include "inc/physics/polygon.hpp"
#include "inc/sound.hpp"

#define WAV_PATH "assets/minor_clam.wav"

const std::chrono::milliseconds timePerFrame(16);

void sleepForTimeLeft(std::chrono::time_point<std::chrono::high_resolution_clock> startTime) {
    auto end = std::chrono::high_resolution_clock::now();
    if (end - startTime < timePerFrame)
        std::this_thread::sleep_for(timePerFrame - (end - startTime));
}

void display(std::atomic<bool> *quit, ThreadManager *manager)
{
    DisplayManager displayManager("Test Window");

    if (!displayManager.isInitialized()) {
        std::cout << "Display Manager failed" << std::endl;
        return;
    }
    manager->openBuffer(buffers::render);

    while (!(*quit)) {
        auto start = std::chrono::high_resolution_clock::now();
        while (manager->newMessages(buffers::render)) {
            auto&& msg = manager->getMessage<RenderMessage>(buffers::render);
            displayManager.addRenderable(std::move(msg));
        }

        displayManager.displayAll();
        sleepForTimeLeft(start);
    }
}

void physics(std::atomic<bool> *quit, ThreadManager *manager)
{
    manager->openBuffer(buffers::input);
    manager->openBuffer(buffers::createBody);

    phy::World world(Vec2<float>(5, 9.8), manager);

    while (!(*quit)) {
        auto start = std::chrono::high_resolution_clock::now();

        while (manager->newMessages(buffers::createBody)) {
            auto &&msg = manager->getMessage<CreateBodyMessage>(buffers::createBody);
            auto bod = world.createBody(msg->bodySpec);

            manager->sendMessage(buffers::bodyCreated,
                                 std::make_unique<BodyCreatedMessage>(bod, msg->type));
        }

        while (manager->newMessages(buffers::input)) {
            auto &&msg = manager->getMessage<InputMessage>(buffers::input);
            msg->command->execute();
        }

        world.step();
        manager->sendMessage(buffers::render, world.getObjects());

        sleepForTimeLeft(start);
    }
}

void audio(std::atomic<bool> *quit, ThreadManager *manager)
{
    manager->openBuffer(buffers::sound);

    SoundManager soundManager;
    auto effect = soundManager.addSound("assets/high.wav", SOUND_TYPE::EFFECT);
    auto music = soundManager.addSound("assets/minor_clam.wav", SOUND_TYPE::MUSIC);

    music->playSound(127);

    while (!(*quit)) {
        auto start = std::chrono::high_resolution_clock::now();

        while (manager->newMessages(buffers::sound)) {
            auto &&msg = manager->getMessage<AudioMessage>(buffers::sound);
            effect->playSound(msg->posLeft);
        }

        sleepForTimeLeft(start);
    }
}

void events(std::atomic<bool> *quit, ThreadManager *manager)
{
    manager->openBuffer(buffers::bodyCreated);

    EventHandler eventHandler(manager);
    if (!eventHandler.isInitialized()) {
        std::cout << "EventHandler failed" << std::endl;
        return;
    }

    phy::BodySpec spec;
    spec.bodyType = phy::BodyType::dynamicBody;
    spec.position = {180, 180};
    spec.gravityFactor = 1;
    auto shape = phy::PolygonShape(1.0f);
    shape.setBox(Vec2<float>(25, 25));
    spec.shapes.push_back(std::make_shared<phy::PolygonShape>(shape));

    manager->sendMessage(buffers::createBody,
                         std::make_unique<CreateBodyMessage>(spec, CharacterType::Player));

    SDL_Event e{};
    while (!(*quit)) {
        auto start = std::chrono::high_resolution_clock::now();
        if (eventHandler.inputHandler(e) == 1) {
            manager->waitAll();
            return;
        }

        if (manager->newMessages(buffers::bodyCreated)) {
            auto&& body = manager->getMessage<BodyCreatedMessage>(buffers::bodyCreated);
            if (!eventHandler.getPlayer().lock())
                eventHandler.setPlayer(body->body);
        }

        eventHandler.executeEvents();

        sleepForTimeLeft(start);
    }
}

int main(int argc, char **args)
{
    ThreadManager threadManager;

    threadManager.spawnThread(display);
    threadManager.spawnThread(physics);
    threadManager.spawnThread(audio);
    std::this_thread::sleep_for(timePerFrame);
    events(&threadManager.stopThreads, &threadManager);

    threadManager.waitAll();

    return 0;
}
