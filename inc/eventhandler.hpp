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

class EventHandler
{
private:
    bool initialized;
    std::array<bool, static_cast<char>(Commands::NUM_OF_COMMANDS)> commandState{};
    std::map<SDL_Keycode, Commands> keysToCommands;
    std::map<uint8_t, Commands> buttonsToCommands;
    std::weak_ptr<phy::Body> body;
    Controller controller;
    int camPosX;

    class Command {
        public:
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
        EventHandler* eventHandler;
      public:
        MoveCommand(EventHandler* eventhandler){
          eventHandler = eventhandler;
        }
        void addCommand(Vec2<int> addVel) {
          addVelocity = addVel;
          eventHandler->eventStack.push(eventHandler->moveCommand);
        }
        virtual void execute() const override {
          eventHandler->addPlayerVel(addVelocity);
        }
    };
    class ActionCommand : public Command {
        public:
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
    void actionHandler(Commands command, bool pressed);
    void initKeyMapping();
    void initButtonMapping();
    std::queue<Command*> eventStack;
    JumpCommand* jumpCommand;
    MoveCommand* moveCommand;
    ActionCommand* actionCommand;
    SpecialCommand* specialCommand;
    DuckCommand* duckCommand;
public:
    EventHandler();
    ~EventHandler();
    bool isInitialized() const;
    int inputHandler(SDL_Event &event);
    void addEvent(Command &newCommand);
    void executeEvents();
    Command *getCommandPtr(Commands cmd);
    void setPlayer(std::weak_ptr<phy::Body> bodyPtr);
    void addPlayerVel(Vec2<int> addVelocity);
    int getSoundOrigin();
    void setCamPosX(int camX);
};