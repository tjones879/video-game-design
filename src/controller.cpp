#include "inc/controller.hpp"

Controller::Controller(int joystick)
    : controller(nullptr), initialized(false), deadzone(5000)
{
    setJoystick(joystick);
}

Controller::Controller()
    : controller(nullptr), initialized(false), deadzone(5000)
{
}

bool Controller::setJoystick(int joystick)
{
    if (!controller) {
        controller = SDL_GameControllerOpen(joystick);
        if (controller) {
            name = SDL_GameControllerName(controller);
            mapping = std::string(SDL_GameControllerMapping(controller));
            initialized = true;
        }
    }
    return initialized;
}

Controller::~Controller()
{
    if (controller)
        SDL_GameControllerClose(controller);
}

float Controller::getAnalog(SDL_ControllerAxisEvent event) const
{
    float magnitude = 0.0f;
    const int16_t maxMagnitude = INT16_MAX - deadzone;
    if (event.value < (-1 * deadzone)) {
        magnitude = (double) (event.value + deadzone) / (double) (maxMagnitude);
    } else if(event.value > deadzone) {
        magnitude = (double) (event.value - deadzone) / (double) (maxMagnitude);
    }
    return magnitude;
}

Controller::operator const SDL_GameController*() const
{
    return controller;
}

Controller::operator SDL_JoystickID() const
{
    auto js = SDL_GameControllerGetJoystick(controller);
    return SDL_JoystickInstanceID(js);
}

std::ostream &operator<<(std::ostream &out, const Controller &ctrl)
{
    if (!ctrl.initialized) {
        out << "FAILURE TO INITIALIZE" << std::endl;
    } else {
        out << "Name: " << ctrl.name << std::endl;
        out << "Mapping: " << ctrl.mapping << std::endl;
    }
    return out;
}
