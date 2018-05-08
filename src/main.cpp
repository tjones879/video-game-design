#include <chrono>
#include "inc/common.hpp"
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
#include <unordered_set>


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
    manager->openBuffer(buffers::destroyBody);

    phy::World world(Vec2<float>(0, 0), manager);

    while (!(*quit)) {
        auto start = std::chrono::high_resolution_clock::now();

        while (manager->newMessages(buffers::createBody)) {
            auto &&msg = manager->getMessage<CreateBodyMessage>(buffers::createBody);
            auto bod = world.createBody(msg->bodySpec);

            manager->sendMessage(buffers::bodyCreated,
                                 std::make_unique<BodyCreatedMessage>(bod, msg->type));
        }

        while (manager->newMessages(buffers::destroyBody)) {
            auto &&msg = manager->getMessage<DestroyBodyMessage>(buffers::destroyBody);
            world.destroyBody(msg->body);
        }

        while (manager->newMessages(buffers::input)) {
            auto &&msg = manager->getMessage<InputMessage>(buffers::input);
            msg->command->execute();
        }

        world.step();
        manager->sendMessage(buffers::render, world.getObjects());
        manager->sendMessage(buffers::collisions, world.getCollisions());

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
    manager->openBuffer(buffers::collisions);

    EventHandler eventHandler(manager);
    if (!eventHandler.isInitialized()) {
        std::cout << "EventHandler failed" << std::endl;
        return;
    }

    // Create Projectlie & Player
    phy::BodySpec spec;
    spec.bodyType = phy::BodyType::dynamicBody;
    spec.position = {80, 80};
    spec.gravityFactor = 1;
    auto shape = phy::CircleShape(1.0f, 15, {0, 0});
    spec.shapes.push_back(std::make_shared<phy::CircleShape>(shape));


    manager->sendMessage(buffers::createBody,
                         std::make_unique<CreateBodyMessage>(spec, CharacterType::Player));
    manager->sendMessage(buffers::createBody,
                         std::make_unique<CreateBodyMessage>(spec, CharacterType::Projectile));

    // Create Spawner
    spec.position = {250, 250};
    spec.gravityFactor = 0;
    auto shape3 = phy::PolygonShape(1.0f);
    shape3.setBox(Vec2<float>(40, 40));
    spec.shapes.clear();
    spec.shapes.push_back(std::make_shared<phy::PolygonShape>(shape3));
    manager->sendMessage(buffers::createBody,
                         std::make_unique<CreateBodyMessage>(spec, CharacterType::Spawner));

    // Create Enemies
    auto enemies = eventHandler.defineEnemies(50);
    for (auto spec : enemies)
        manager->sendMessage(buffers::createBody,
                             std::make_unique<CreateBodyMessage>(spec, CharacterType::Enemy));

    // Create Walls
    auto boundaries = eventHandler.defineBoundaries(Vec2<float>(250, -100), 25, 400);
    for (auto boundary : boundaries)
        manager->sendMessage(buffers::createBody,
                             std::make_unique<CreateBodyMessage>(boundary, CharacterType::Boundary));


    SDL_Event e{};
    while (!(*quit)) {
        auto start = std::chrono::high_resolution_clock::now();
        if (eventHandler.inputHandler(e) == 1) {
            manager->waitAll();
            return;
        }

        if (manager->newMessages(buffers::bodyCreated)) {
            auto&& msg = manager->getMessage<BodyCreatedMessage>(buffers::bodyCreated);
            switch(msg->type) {
            case CharacterType::Player:
                eventHandler.setPlayer(msg->body);
                break;
            case CharacterType::Enemy:
                eventHandler.addEnemy(msg->body.lock());
                break;
            case CharacterType::Spawner:
                eventHandler.setSpawner(msg->body);
                break;
            case CharacterType::Boundary:
                eventHandler.addBoundary(msg->body);
                break;
            case CharacterType::Projectile:
                eventHandler.setProjectile(msg->body);
            case CharacterType::Unknown:
                break;
            }
        }

        if (manager->newMessages(buffers::collisions)) {
            auto&& msg = manager->getMessage<CollisionMessage>(buffers::collisions);
            for (auto bodyPair : msg->bodies) {
                // Check if one of the bodies is a boundary
                auto index = eventHandler.boundaryCollision(bodyPair);
                if (index == 1 || index == 2) {
                    std::weak_ptr<phy::Body> body;
                    if (index == 1)
                        body = bodyPair.second;
                    else if (index == 2)
                        body = bodyPair.first;

                    auto vel = body.lock()->getLinearVelocity();
                    auto extra = body.lock()->getExtraData();
                    if (!extra->colliding && body.lock() != eventHandler.getProjectile().lock()) {
                        extra->colliding = true;
                        auto cmd = std::make_unique<MoveCommand>(body, Vec2<float>(-2.5 * vel.x, -2.5 * vel.y),
                                [](std::weak_ptr<phy::Body> body) {
                                    body.lock()->getExtraData()->colliding = false;
                                });
                        manager->sendMessage(buffers::input,
                                             std::make_unique<InputMessage>(std::move(cmd)));
                    }
                } // Check if one of the bodies is the projectile

                else if ((index = eventHandler.projectileCollision(bodyPair)) && index) {
                    std::weak_ptr<phy::Body> enemy;
                    std::weak_ptr<phy::Body> playerAttack;
                    if (index == 1){
                        enemy = bodyPair.second;
                        playerAttack = bodyPair.first;
                    }
                    else{
                        enemy = bodyPair.first;
                        playerAttack = bodyPair.second;
                    }
                    auto enemies = eventHandler.getEnemies();
                    auto search = enemies.find(enemy.lock());
                    if(search != enemies.end()){
                        std::cout<<"Enemy: "<<enemy.lock()->getExtraData()->color.a<<std::endl;
                        std::cout<<"Attack: "<<playerAttack.lock()->getExtraData()->color.a<<std::endl;
                    } 
                    // TODO: Color Logic
                }
            }
        }

        eventHandler.executeEvents();

        auto color = eventHandler.setPlayerColor();
        auto projectile = eventHandler.getProjectile().lock();
        if (projectile->getExtraData() && !projectile->getExtraData()->expanding)
            projectile->getExtraData()->color = color;

        if (projectile && eventHandler.getPlayer().lock())
            projectile->setPosition(eventHandler.getPlayer().lock()->getPosition());

        sleepForTimeLeft(start);
    }
}

int main(int argc, char **args)
{
    std::srand(std::time(0));
    ThreadManager threadManager;

    threadManager.spawnThread(display);
    threadManager.spawnThread(physics);
    threadManager.spawnThread(audio);
    std::this_thread::sleep_for(timePerFrame);
    events(&threadManager.stopThreads, &threadManager);

    threadManager.waitAll();

    return 0;
}
