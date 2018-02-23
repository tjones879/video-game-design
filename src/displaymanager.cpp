#include <inc/displaymanager.hpp>
#include <iostream>

#define DEBUG(e) std::cerr << e << std::endl;

DisplayManager::DisplayManager(const std::string &title)
    : window(NULL), initialized(false)
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER)<0){
		std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
	}else{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
			std::cout<<"Warning: Linear texture filtering not enabled!"<<std::endl;
		}
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
								  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
								  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (!window){
			std::cout << "SDL Window Error: " << SDL_GetError() << std::endl;
		}else{
			renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
			if(!renderer){
				std::cout << "Renderer could not be created. Error: " << SDL_GetError() << std::endl;
			}else{
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
			}
		}
	}
	initialized = true;
}

DisplayManager::~DisplayManager()
{
    SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;
}

SDL_Window *DisplayManager::operator()() const
{
    return window;
}

bool DisplayManager::isInitialized() const
{
    return initialized;
}

bool DisplayManager::loadMedia(){
	bool success = true;
    testSurface = SDL_LoadBMP("../assets/hello_world.bmp");
    if (!testSurface) {
        std::cout << "Unable to load image: " << SDL_GetError() << std::endl;
        success = false;
    }

    return success;
}

void DisplayManager::renderRect(const SDL_Rect* rect, const int red, const int blue, const int green, const int alpha){
	SDL_SetRenderDrawColor( renderer, red, blue, green, alpha );		
	SDL_RenderFillRect( renderer, rect );
}

SDL_Renderer* DisplayManager::getRenderer(){
	return renderer;
}

void DisplayManager::renderScreen(){
	SDL_RenderPresent( renderer );
}

void DisplayManager::clearScreen(){
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( renderer );
}

