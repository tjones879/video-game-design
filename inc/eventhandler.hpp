#pragma once

#include "inc/controller.hpp"

#include <SDL2/SDL.h>
#include <array>
#include <map>
#include <queue>
#include <iostream>

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
    Controller controller;
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
        public:
          virtual void execute() const {
              //Update object position
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
    void addEvent(Command &newEvent);
    void executeEvents();
    Command *getCommandPtr(Commands cmd);
};
