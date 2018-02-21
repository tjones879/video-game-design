#pragma once

#include <SDL2/SDL.h>
#include <array>
#include <map>
#include <queue>
#include <iostream>

#define DEBUG(e) std::cerr << e << std::endl;

class EventHandler
{
private:
    bool initialized;
    std::array<bool,285> keyState{};
    enum commands{
        JUMP,
        DUCK,
        BACK,
        FORWARD,
        ACTION,
        SPECIAL,
        QUIT,
        NUM_OF_COMMANDS
    };
    std::map<int,SDL_Keycode> keysToCommands; 
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
    Command* getCommandPtr(int);
};
