#pragma once

#include "inc/controller.hpp"
#include "inc/sound.hpp"
#include <SDL2/SDL_mixer.h>

#define WAV_PATH "assets/scratch.wav"
#include <SDL2/SDL.h>
#include <array>
#include <map>
#include <queue>
#include <iostream>
#include <inc/vec2.hpp>
#include <inc/displaymanager.hpp>
#include <inc/physics/body.hpp>
#include "inc/threadmanager.hpp"

#define DEBUG(e) std::cerr << e << std::endl;

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
    Vec2<int> addVelocity;
    std::weak_ptr<phy::Body> body;
    void addPlayerVel() const {
        auto locked = body.lock();
        auto currVel = locked->getLinearVelocity();
        locked->setLinearVelocity(currVel + addVelocity);
    }

public:
    MoveCommand(std::weak_ptr<phy::Body> body, Vec2<int> addVel){
        this->body = body;
        addVelocity = addVel;
    }

    virtual void execute() const override {
        addPlayerVel();
    }
};

class ActionCommand : public Command {
    std::weak_ptr<phy::Body> body;
public:
    ActionCommand(std::weak_ptr<phy::Body> body) {
        this->body = body;
        type = Commands::ACTION;
    }
    virtual void execute() const {
        //action();
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
    std::weak_ptr<phy::Body> body;
    std::queue<std::unique_ptr<Command>> eventStack;
    Controller controller;
    int camPosX;
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
    std::weak_ptr<const phy::Body> getPlayer() const;
    int getSoundOrigin();
    void setCamPosX(int camX);
};
