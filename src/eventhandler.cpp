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
				switch (keysToCommands[event.key.keysym.sym]){
					case JUMP:
						DEBUG("Jump");
						keyState[event.key.keysym.scancode]=true;
						eventStack.push_back(jumpCommand);
						return 0;
					case DUCK:
						keyState[event.key.keysym.scancode]=true;
						eventStack.push_back(duckCommand);
						DEBUG("Duck");
						return 0;
					case BACK:
						keyState[event.key.keysym.scancode]=true;
						eventStack.push_back(moveCommand);
						DEBUG("Back");
						return 0;
					case FORWARD:
						keyState[event.key.keysym.scancode]=true;
						eventStack.push_back(moveCommand);
						DEBUG("Forward");
						return 0;
					case ACTION:
						DEBUG("Action");
						keyState[event.key.keysym.scancode]=true;
						eventStack.push_back(actionCommand);
						return 0;
					case SPECIAL:
						DEBUG("Special");
						keyState[event.key.keysym.scancode]=true;
						eventStack.push_back(specialCommand);
						return 0;
					case QUIT:
						DEBUG("Quit");
						return 1;
				}
			case SDL_KEYUP: 
				switch (keysToCommands[event.key.keysym.sym]){
					case JUMP:
						keyState[event.key.keysym.scancode]=false;
						return 0;
					case DUCK:
						keyState[event.key.keysym.scancode]=false;
						return 0;
					case BACK:
						keyState[event.key.keysym.scancode]=false;
						return 0;
					case FORWARD:
						keyState[event.key.keysym.scancode]=false;
						return 0;
					case ACTION:
						keyState[event.key.keysym.scancode]=false;
						return 0;
					case SPECIAL:
						keyState[event.key.keysym.scancode]=false;
						return 0;
				}
		}
	}
}

bool EventHandler::isInitialized() const
{
    return initialized;
}
