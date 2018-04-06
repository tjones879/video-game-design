#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include "inc/vec2.hpp"
#include "inc/eventhandler.hpp"
#include "inc/displaymanager.hpp"
#include "inc/physics/world.hpp"
#include "inc/physics/common.hpp"
#include "inc/physics/polygon.hpp"
#include "inc/sound.hpp"
#include "sound.cpp"
#define WAV_PATH "assets/scratch.wav"
//Mix_Chunk *wave = NULL;
const int MIN_MILLISECONDS_PER_FRAME = 16;

int main(int argc, char **args)
{
    DisplayManager displayManager("Test Window");
    if (!displayManager.isInitialized()) {
        std::cout << "Display Manager failed" << std::endl;
        return 1;
    }
    Sound::init();

    Sound* effect = new Sound(WAV_PATH, SOUND_EFFECT);

    EventHandler eventHandler;
    if (!eventHandler.isInitialized()) {
        std::cout << "EventHandler failed" << std::endl;
        return 1;
    }

    //Initialize SDL_mixer 
    //if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        //std::cout << "Error in Mix_OpenAudio" << std::endl;
        //return -1;
    //



    // Load our sound effect
    //wave = Mix_LoadWAV("assets/scratch.wav");
    //if (wave == NULL) {
        //std::cout << "Error in Mix_LoadWAV" << std::endl;
        //return -1;
    //}

    phy::World world(Vec2<float>(5, 9.8));


    phy::BodySpec spec;
    spec.bodyType = phy::BodyType::dynamicBody;
    spec.position = {100, 100};
    spec.gravityFactor = 1;
    auto shape = phy::PolygonShape(1.0f);
    shape.setBox(Vec2<float>(25, 25));
    auto body = world.createBody(spec);
    auto shape_ptr = body.lock()->addShape(shape);

    phy::BodySpec spec1;
    spec1.bodyType = phy::BodyType::dynamicBody;
    spec1.position = {-10, -10};
    spec1.gravityFactor = 5;
    auto shape1 = phy::PolygonShape(1.0f);
    shape1.setBox(Vec2<float>(25, 25));
    auto body1 = world.createBody(spec1);
    auto shape_ptr1 = body1.lock()->addShape(shape1);


    std::vector<std::weak_ptr<phy::Body>> bodies;
    bodies.push_back(body);
    bodies.push_back(body1);
    std::vector<std::weak_ptr<phy::PolygonShape>> shapes;
    shapes.push_back(shape_ptr);
    shapes.push_back(shape_ptr1);
    //if ( Mix_PlayChannel(-1, wave, 0) == -1 )
        //return -1;
    effect->playSound();
    bool quit = false;
    SDL_Event e{};
    while (!quit) {
        const int start = SDL_GetTicks();
        displayManager.displayPolygon(bodies, shapes);
        if (eventHandler.inputHandler(e) == 1)
            return 0;
        eventHandler.executeEvents();

        world.step();

        const int millisecondsThisFrame = SDL_GetTicks() - start;
        if (millisecondsThisFrame < MIN_MILLISECONDS_PER_FRAME) {
            // If rendering faster than 60FPS, delay
            SDL_Delay(MIN_MILLISECONDS_PER_FRAME - millisecondsThisFrame);
        }
    }
    return 0;
}
