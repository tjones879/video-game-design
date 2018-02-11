#include <inc/eventhandler.hpp>
#include <iostream>

EventHandler::EventHandler(){
	initialized = true;
}

EventHandler::~EventHandler(){
}

EventHandler::InputHandler(SDL_Event &event){
	while(SDL_PollEvent(&event) != 0){
		if (e.type == SDL_QUIT) quit = true;
		switch(event.type){
			case SDL_KEYDOWN: std::cout << "Keypress detected"<<std::endl;
		}
	}
}

bool EventHandler::isInitialized() const
{
    return initialized;
}
