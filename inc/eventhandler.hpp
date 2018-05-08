#pragma once

#include "inc/controller.hpp"
#include "inc/sound.hpp"
#include "inc/common.hpp"
#include <SDL2/SDL_mixer.h>

#include <SDL2/SDL.h>
#include <array>
#include <map>
#include <queue>
#include <iostream>
#include <inc/vec2.hpp>
#include <inc/displaymanager.hpp>
#include <inc/physics/body.hpp>
#include "inc/threadmanager.hpp"
#include <unordered_set>
#include <functional>

enum class Commands : char {
    JUMP,
    DUCK,
    BACK,
    FORWARD,
    ACTION,
    SPECIAL,
    QUIT,
    NUM_OF_COMMANDS
};

class Command {
public:
    Commands type;
    virtual ~Command() {};
    virtual void execute() const = 0;
};

class JumpCommand : public Command {
public:
    virtual void execute() const {
        DEBUG("Execute Jump");
        //jump();
        return;
    }
};

class DuckCommand : public Command {
public:
    virtual void execute() const {
        //duck();
    }
};

class MoveCommand : public Command {
private:
    Vec2<float> addVelocity;
    float dt; ///< The length of time the body should move at its new velocity
    std::weak_ptr<phy::Body> body;
    std::function<void (std::weak_ptr<phy::Body>)> runAfter;
    void addPlayerVel() const {
        auto locked = body.lock();
        auto currVel = locked->getLinearVelocity();
        locked->setLinearVelocity(currVel + addVelocity);
    }

public:
    MoveCommand(std::weak_ptr<phy::Body> body, Vec2<float> addVel, std::function<void (std::weak_ptr<phy::Body>)> lambda=0){
        this->body = body;
        addVelocity = addVel;
        runAfter = lambda;
    }

    virtual void execute() const override {
        addPlayerVel();
        if (runAfter)
            runAfter(body);
        body.lock()->updatePosition(1.f / 30.f);
    }
};

class ActionCommand : public Command {
    std::weak_ptr<phy::Body> body;
public:
    ActionCommand(std::weak_ptr<phy::Body> bod)
        : body(bod) {
            type = Commands::ACTION;
    }

    virtual void execute() const {
        auto expand = &body.lock()->getExtraData()->expanding;
        std::cout << "Old expand: " << *expand << std::endl;
        if (!(*expand))
            *expand = 1;
        else
            *expand *= -1;
        std::cout << "New expand: " << *expand << std::endl;
    }
};

class SpecialCommand : public Command {
  public:
    virtual void execute() const {
        //special();
    }
};

class EventHandler
{
private:
    ThreadManager *threadManager;
    bool initialized;
    std::array<bool, static_cast<char>(Commands::NUM_OF_COMMANDS)> commandState{};
    std::map<SDL_Keycode, Commands> keysToCommands;
    std::map<uint8_t, Commands> buttonsToCommands;
    std::weak_ptr<phy::Body> player;
    std::weak_ptr<phy::Body> spawner;
    std::vector<std::weak_ptr<phy::Body>> boundaries;
    std::weak_ptr<phy::Body> projectile;
    std::queue<std::unique_ptr<Command>> eventStack;
    std::unordered_set<std::shared_ptr<phy::Body>> enemies;
    Controller controller;

    int camPosX;
    int colorAngle;
    bool angleIncrement = true;
    void actionHandler(Commands command, bool pressed);
    void initKeyMapping();
    void initButtonMapping();
public:
    EventHandler(ThreadManager *manager);
    ~EventHandler();
    bool isInitialized() const;
    int inputHandler(SDL_Event &event);
    void addEvent(Command &newCommand);
    void executeEvents();
    void setPlayer(std::weak_ptr<phy::Body> bodyPtr);
    void setSpawner(std::weak_ptr<phy::Body> bodyPtr);
    std::weak_ptr<const phy::Body> getSpawner() const;
    std::weak_ptr<phy::Body> getPlayer();
    int getSoundOrigin();
    void setCamPosX(int camX);

    std::vector<phy::BodySpec>
    defineBoundaries(Vec2<float> center, float thickness, float sideLength) const;
    void addBoundary(std::weak_ptr<phy::Body> boundary);
    /**
     * Determine if the given collision involves a body.
     *
     * @return 0 if not involved with a boundary
     *         1 if the first body is a boundary
     *         2 if the second body is a boundary
     *         3 if both bodies are boundaries (this is an invalid state)
     */
    int boundaryCollision(std::pair<std::weak_ptr<phy::Body>, std::weak_ptr<phy::Body>> bodies);
    /**
     * Create a random amount of enemies and return them.
     */
    std::vector<phy::BodySpec> defineEnemies(uint32_t numEnemies) const;
    /**
     *
     */
    std::unordered_set<std::shared_ptr<phy::Body>> getEnemies() const;
    /**
     * Add a created body to the enemy list for tracking.
     */
    void addEnemy(std::shared_ptr<phy::Body> enemy);
    /**
     * Determine if the given collision involves a projectile.
     *
     * @return 0 if not involved with the projectile
     *         1 if the first body is the projectile
     *         2 if the second body is the projectile
     */
    int projectileCollision(std::pair<std::weak_ptr<phy::Body>, std::weak_ptr<phy::Body>> bodyPair);
    void setProjectile(std::weak_ptr<phy::Body> proj);
    std::weak_ptr<phy::Body> getProjectile();
    SDL_Color setPlayerColor();
};
