#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <SDL2/SDL_gamecontroller.h>

class Controller {
    SDL_GameController *controller;
    bool initialized;
    std::string name;
    std::string mapping;
    std::vector<bool> buttons;
    std::vector<int16_t> axis;
public:
    /**
     * Initialize a game controller with the given joystick id.
     */
    Controller(int joystick);
    /**
     * Create a controller without intializing it.
     */
    Controller();
    /**
     * Attempt to assign this controller a specific joystick id.
     *
     * The joystick id should be in the range [0, SDL_NumJoysticks()].
     *
     * @note This has no effect if a joystick is already set.
     * @return Whether or not opening the controller was successful.
     */
    bool setJoystick(int joystick);
    ~Controller();
    friend std::ostream &operator<<(std::ostream &out, const Controller &controller);
};

std::ostream &operator<<(std::ostream &out, const Controller &controller);
