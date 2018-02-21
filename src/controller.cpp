#include "inc/controller.hpp"

Controller::Controller(int joystick)
    : controller(nullptr), initialized(false)
{
    setJoystick(joystick);
}

Controller::Controller()
    : controller(nullptr), initialized(false)
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
