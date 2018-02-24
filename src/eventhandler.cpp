#include <inc/eventhandler.hpp>
#include <iostream>
#include <stdio.h>

#define DEBUG(e) std::cerr << e << std::endl;

EventHandler::EventHandler(){
    initialized = true;
    commandState.fill(false);
    keysToCommands[SDLK_UP]=JUMP;
    keysToCommands[SDL_CONTROLLER_BUTTON_A]=JUMP;
    keysToCommands[SDL_CONTROLLER_BUTTON_DPAD_UP]=JUMP;
    keysToCommands[SDLK_DOWN]=DUCK;
    keysToCommands[SDL_CONTROLLER_BUTTON_DPAD_DOWN]=DUCK;
    keysToCommands[SDLK_LEFT]=BACK;
    keysToCommands[SDL_CONTROLLER_BUTTON_DPAD_LEFT]=BACK;
    keysToCommands[SDLK_RIGHT]=FORWARD;
    keysToCommands[SDL_CONTROLLER_BUTTON_DPAD_RIGHT]=FORWARD;
    keysToCommands[SDLK_f]=ACTION;
    keysToCommands[SDL_CONTROLLER_BUTTON_B]=ACTION;
    keysToCommands[SDLK_d]=SPECIAL;
    keysToCommands[SDL_CONTROLLER_BUTTON_X]=SPECIAL;
    keysToCommands[SDLK_ESCAPE]=QUIT;
    duckCommand = new DuckCommand;
    specialCommand = new SpecialCommand;
    actionCommand = new ActionCommand;
    moveCommand = new MoveCommand;
    jumpCommand = new JumpCommand;

    // Temporarily open the first joystick to the controller if it exists
    if (SDL_NumJoysticks() > 0)
        ctrl.setJoystick(0);
}

EventHandler::~EventHandler(){
	delete jumpCommand;
    delete moveCommand;
    delete actionCommand;
    delete specialCommand;
    delete duckCommand;
}

void EventHandler::actionHandler(const int command, const int flag=0, const float move=0){
	switch (command) {
            case JUMP:
                DEBUG("Jump");
                if(flag==(KEYDOWN||JOYDOWN)) commandState[JUMP] = true;
                if(flag==KEYUP||JOYUP) commandState[JUMP] = false;
                eventStack.push(jumpCommand);
                return;
            case DUCK:
                DEBUG("Duck");
                if(flag==(KEYDOWN||JOYDOWN)) commandState[DUCK] = true;
                if(flag==KEYUP||JOYUP) commandState[DUCK] = false;
                eventStack.push(duckCommand);
                return;
            case BACK:
                DEBUG("Back");
                if(flag==(KEYDOWN||JOYDOWN)) commandState[BACK] = true;
                if(flag==KEYUP||JOYUP) commandState[BACK] = false;
                eventStack.push(moveCommand);
                return;
            case FORWARD:
                DEBUG("Forward");
                if(flag==(KEYDOWN||JOYDOWN)) commandState[FORWARD] = true;
                if(flag==KEYUP||JOYUP) commandState[FORWARD] = false;
                eventStack.push(moveCommand);
                return;
            case ACTION:
                DEBUG("Action");
                if(flag==(KEYDOWN||JOYDOWN)) commandState[ACTION] = true;
                if(flag==KEYUP||JOYUP) commandState[ACTION] = false;
                eventStack.push(actionCommand);
                return;
            case SPECIAL:
                DEBUG("Special");
                if(flag==(KEYDOWN||JOYDOWN)) commandState[SPECIAL] = true;
                if(flag==KEYUP||JOYUP) commandState[SPECIAL] = false;
                eventStack.push(specialCommand);
                return;
            }
}

int EventHandler::inputHandler(SDL_Event &event){
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT)
            return 1;
		if (keysToCommands.find(event.cbutton.button) == keysToCommands.end()||
			keysToCommands.find(event.key.keysym.sym) == keysToCommands.end()) 
            return 0;
		DEBUG("Keypress detected. Key: "<<event.key.keysym.scancode<<" & command: "<<keysToCommands[event.key.keysym.sym]
				<<" & command state: "<<commandState[keysToCommands[event.key.keysym.sym]]);
        switch (event.type) {
			case SDL_KEYDOWN: 
				if (keysToCommands.find(event.key.keysym.sym) == keysToCommands.end()||
					commandState[keysToCommands[event.key.keysym.sym]]==true)
					return 0;
				if (keysToCommands[event.key.keysym.sym]==QUIT)
					return 1;
				actionHandler(keysToCommands[event.key.keysym.sym],KEYDOWN);
			case SDL_KEYUP:
				if (keysToCommands.find(event.key.keysym.sym) == keysToCommands.end()||
					commandState[keysToCommands[event.key.keysym.sym]]==false)
					return 0;
				actionHandler(keysToCommands[event.key.keysym.sym],KEYUP);
			case SDL_CONTROLLERAXISMOTION:
				return 0;
			case SDL_CONTROLLERBUTTONDOWN:
				if (keysToCommands.find(event.cbutton.button) == keysToCommands.end()||
					commandState[keysToCommands[event.cbutton.button]]==true)
					return 0;
				actionHandler(keysToCommands[event.cbutton.button],JOYDOWN);
			case SDL_CONTROLLERBUTTONUP:
				if (keysToCommands.find(event.cbutton.button) == keysToCommands.end()||
					commandState[keysToCommands[event.cbutton.button]]==false)
					return 0;
				actionHandler(keysToCommands[event.cbutton.button],JOYUP);
        }
    }
    return 0;
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
    while (!eventStack.empty()) {
        eventStack.front()->execute();
        eventStack.pop();
    }
}

auto EventHandler::getCommandPtr(int cmd) -> Command*{
    switch (cmd) {
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
