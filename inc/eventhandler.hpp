#pragma once

#include <SDL2/SDL.h>
#include <array>
#include <map>
#include <vector>
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
		  virtual void execute() = 0;
	};
	class JumpCommand : public Command {
		public:
		  virtual void execute() { 
			  DEBUG("Execute Jump");	
			  //jump(); 
			  return;
		  }
	};
	class DuckCommand : public Command {
		public:
		  virtual void execute() { 
			  //duck(); 
		  }
	};
	class MoveCommand : public Command {
		public:
		  virtual void execute() { 
			  //Need to have object pointer
			  //Update object position 
		  }
	};
	class ActionCommand : public Command {
		public:
		  virtual void execute() { 
			  //action(); 
		  }
	};
	class SpecialCommand : public Command {
		public:
		  virtual void execute() { 
			  //special(); 
		  }
	};
	std::vector<Command*> eventStack;
	JumpCommand* jumpCommand = new JumpCommand;
	MoveCommand* moveCommand = new MoveCommand;
	ActionCommand* actionCommand = new ActionCommand;
	SpecialCommand* specialCommand = new SpecialCommand;
	DuckCommand* duckCommand = new DuckCommand;
public:
    EventHandler();
    ~EventHandler();
    bool isInitialized() const;
    int inputHandler(SDL_Event &event);
    void addEvent();
};
