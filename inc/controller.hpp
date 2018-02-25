#pragma once

#include <utility>
#include <string>
#include <vector>
#include <iostream>
#include <stdint.h>

#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_events.h>

struct Axis {
    SDL_GameControllerAxis *axis; ///< SDL Axis enumeration
    float value; ///< Analog value in the range [-1, 1]
};

class Controller {
    SDL_GameController *controller;
    bool initialized;
    std::string name;
    std::string mapping;
    std::pair<Axis, Axis> thumbstick;
    int deadzone;
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
    /**
     * Get the state of the axis on the controller.
     *
     * @return The specific axis and its value
     */
    std::pair<Axis, Axis> queryThumbstick();
    /**
     * Update the controller state with an axis event.
     */
    void updateAxis(SDL_ControllerAxisEvent event);
    ~Controller();
    operator const SDL_GameController*() const;
    operator SDL_JoystickID() const;
    friend std::ostream &operator<<(std::ostream &out, const Controller &controller);
private:
    /**
     * Subtract the deadzone from the axis event and translate to range [-1, 1]
     */
    float getAnalog(SDL_ControllerAxisEvent event) const;
};

std::ostream &operator<<(std::ostream &out, const Controller &controller);
