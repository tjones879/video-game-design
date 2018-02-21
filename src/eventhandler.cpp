#include <inc/eventhandler.hpp>
#include <iostream>

#define DEBUG(e) std::cerr << e << std::endl;

//.cbutton for joystick

EventHandler::EventHandler(){
	initialized = true;
	keyState.fill(false);
	keysToCommands[SDLK_UP]=JUMP;
	keysToCommands[SDLK_DOWN]=DUCK;
	keysToCommands[SDLK_LEFT]=BACK;
	keysToCommands[SDLK_RIGHT]=FORWARD;
	keysToCommands[SDLK_f]=ACTION;
	keysToCommands[SDLK_d]=SPECIAL;
	keysToCommands[SDLK_ESCAPE]=QUIT;
	duckCommand = new DuckCommand;
	specialCommand = new SpecialCommand;
	actionCommand = new ActionCommand;
	moveCommand = new MoveCommand;
	jumpCommand = new JumpCommand;
}

EventHandler::~EventHandler(){
}

int EventHandler::inputHandler(SDL_Event &event){
	while(SDL_PollEvent(&event) != 0){
		if (event.type == SDL_QUIT) return 1;
		switch(event.type){
			case SDL_KEYDOWN: 
				DEBUG("Keypress detected. Scancode: "<<event.key.keysym.scancode<<" & array value: "<<keyState[event.key.keysym.scancode]);
				if (keyState[event.key.keysym.scancode] == true){
					return 0;
				 }
				if (keysToCommands.find(event.key.keysym.sym)==keysToCommands.end()){
					DEBUG(keysToCommands.find(event.key.keysym.sym)==keysToCommands.end());
					return 0;
				}
				switch (keysToCommands[event.key.keysym.sym]){
					case JUMP:
						DEBUG("Jump");
						keyState[event.key.keysym.scancode]=true;
						eventStack.push(jumpCommand);
						return 0;
					case DUCK:
						keyState[event.key.keysym.scancode]=true;
						eventStack.push(duckCommand);
						DEBUG("Duck");
						return 0;
					case BACK:
						keyState[event.key.keysym.scancode]=true;
						eventStack.push(moveCommand);
						DEBUG("Back");
						return 0;
					case FORWARD:
						keyState[event.key.keysym.scancode]=true;
						eventStack.push(moveCommand);
						DEBUG("Forward");
						return 0;
					case ACTION:
						DEBUG("Action");
						keyState[event.key.keysym.scancode]=true;
						eventStack.push(actionCommand);
						return 0;
					case SPECIAL:
						DEBUG("Special");
						keyState[event.key.keysym.scancode]=true;
						eventStack.push(specialCommand);
						return 0;
					case QUIT:
						DEBUG("Quit");
						return 1;
					default:
						return 0;
				}
			case SDL_KEYUP: 
				switch (keysToCommands[event.key.keysym.sym]){
					case JUMP:
						DEBUG("Keyup: "<<keyState[event.key.keysym.scancode]<<std::endl);
						keyState[event.key.keysym.scancode]=false;
						DEBUG("Keyup: "<<keyState[event.key.keysym.scancode]<<std::endl);
						return 0;
					case DUCK:
						DEBUG("Keyup: "<<keyState[event.key.keysym.scancode]<<std::endl);
						keyState[event.key.keysym.scancode]=false;
						DEBUG("Keyup: "<<keyState[event.key.keysym.scancode]<<std::endl);
						return 0;
					case BACK:
						DEBUG("Keyup: "<<keyState[event.key.keysym.scancode]<<std::endl);
						keyState[event.key.keysym.scancode]=false;
						DEBUG("Keyup: "<<keyState[event.key.keysym.scancode]<<std::endl);
						return 0;
					case FORWARD:
						DEBUG("Keyup: "<<keyState[event.key.keysym.scancode]<<std::endl);
						keyState[event.key.keysym.scancode]=false;
						DEBUG("Keyup: "<<keyState[event.key.keysym.scancode]<<std::endl);
						return 0;
					case ACTION:
						DEBUG("Keyup: "<<keyState[event.key.keysym.scancode]<<std::endl);
						keyState[event.key.keysym.scancode]=false;
						DEBUG("Keyup: "<<keyState[event.key.keysym.scancode]<<std::endl);
						return 0;
					case SPECIAL:
						DEBUG("Keyup: "<<keyState[event.key.keysym.scancode]<<std::endl);
						keyState[event.key.keysym.scancode]=false;
						DEBUG("Keyup: "<<keyState[event.key.keysym.scancode]<<std::endl);
						return 0;
				}
		}
	}
}

bool EventHandler::isInitialized() const
{
    return initialized;
}

void EventHandler::addEvent(Command &newCommand){
	eventStack.push(&newCommand);
	return;
}

void EventHandler::executeEvents(){
	while (!eventStack.empty()){
		eventStack.front()->execute();
		eventStack.pop();
	}
}

auto EventHandler::getCommandPtr(int cmd) -> Command*{
	switch (cmd){
		case JUMP://0
			return jumpCommand;
		case DUCK://1
			return duckCommand;
		case BACK://2
			return moveCommand;
		case FORWARD://3
			return moveCommand;
		case ACTION://4
			return actionCommand;
		case SPECIAL://5
			return actionCommand;
	}
}
