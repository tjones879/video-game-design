#include <inc/eventhandler.hpp>
#include <iostream>

//.cbutton for joystick

EventHandler::EventHandler(){
	initialized = true;
	keystate.fill(const false);
}

EventHandler::~EventHandler(){
}



int EventHandler::InputHandler(SDL_Event &event){
	while(SDL_PollEvent(&event) != 0){
		if (event.type == SDL_QUIT) return 1;
		switch(event.type){
			case SDL_KEYDOWN: 
				std::cout << "Keypress detected. Scancode: "<<event.key.keysym.scancode<<" & array value: "<<keystate[event.key.keysym.scancode]<<std::endl;
				if (keystate[event.key.keysym.scancode] == true){
					return 0;
				 }
				switch (event.key.keysym.sym){
					case SDLK_LEFT:
						std::cout << "Left arrow"<<std::endl;
						return 0;
					case SDLK_RIGHT:
						std::cout << "Right arrow"<<std::endl;
						return 0;
					case SDLK_UP:
						std::cout << "Up arrow"<<std::endl;
						return 0;
					case SDLK_DOWN:
						std::cout << "Down arrow"<<std::endl;
						return 0;
				}
				return 0;
			case SDL_KEYUP: 
				return 0;
		}
	}
}

bool EventHandler::isInitialized() const
{
    return initialized;
}
