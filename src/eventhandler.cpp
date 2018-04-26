#include <inc/messagetypes.hpp>
#include <inc/eventhandler.hpp>
#include <iostream>
#include <cstdio>

#define DEBUG(e) std::cerr << e << std::endl;

void EventHandler::initButtonMapping()
{
    buttonsToCommands[SDL_CONTROLLER_BUTTON_A]=Commands::JUMP;
    buttonsToCommands[SDL_CONTROLLER_BUTTON_DPAD_UP]=Commands::JUMP;
    buttonsToCommands[SDL_CONTROLLER_BUTTON_DPAD_DOWN]=Commands::DUCK;
    buttonsToCommands[SDL_CONTROLLER_BUTTON_DPAD_LEFT]=Commands::BACK;
    buttonsToCommands[SDL_CONTROLLER_BUTTON_DPAD_RIGHT]=Commands::FORWARD;
    buttonsToCommands[SDL_CONTROLLER_BUTTON_B]=Commands::ACTION;
    buttonsToCommands[SDL_CONTROLLER_BUTTON_X]=Commands::SPECIAL;
}

void EventHandler::initKeyMapping()
{
    keysToCommands[SDLK_UP]=Commands::JUMP;
    keysToCommands[SDLK_DOWN]=Commands::DUCK;
    keysToCommands[SDLK_LEFT]=Commands::BACK;
    keysToCommands[SDLK_RIGHT]=Commands::FORWARD;
    keysToCommands[SDLK_f]=Commands::ACTION;
    keysToCommands[SDLK_d]=Commands::SPECIAL;
    keysToCommands[SDLK_ESCAPE]=Commands::QUIT;
}

EventHandler::EventHandler(ThreadManager *manager) : threadManager(manager) {
    initialized = true;
    commandState.fill(false);
    initKeyMapping();
    initButtonMapping();

    // Temporarily open the first joystick to the controller if it exists
    if (SDL_NumJoysticks() > 0) {
        controller.setJoystick(0);
        std::cout << controller << std::endl;
    }
}

EventHandler::~EventHandler(){
}

void EventHandler::actionHandler(Commands command, bool pressed)
{
    std::unique_ptr<Command> cmd;
    if (pressed && !commandState[static_cast<char>(command)]) {
        switch (command) {
        case Commands::JUMP:
            DEBUG("Jump");
            cmd = std::make_unique<MoveCommand>(body, Vec2<int>(0, -3));
            break;
        case Commands::DUCK:
            DEBUG("Duck");
            cmd = std::make_unique<MoveCommand>(body, Vec2<int>(0, 3));
            break;
        case Commands::BACK:
            DEBUG("Back");
            cmd = std::make_unique<MoveCommand>(body, Vec2<int>(-3, 0));
            break;
        case Commands::FORWARD:
            DEBUG("Forward");
            cmd = std::make_unique<MoveCommand>(body, Vec2<int>(3, 0));
            break;
        case Commands::ACTION:
            DEBUG("Action");
            cmd = std::make_unique<MoveCommand>(body, Vec2<int>(0, -3));
            break;
        case Commands::SPECIAL:
            DEBUG("Special");
            cmd = std::make_unique<MoveCommand>(body, Vec2<int>(0, -3));
            break;
        default:
            std::cout << "Invalid button" << std::endl;
            break;
        }
        eventStack.push(std::move(cmd));
    }
    commandState[static_cast<char>(command)] = pressed;
}

int EventHandler::inputHandler(SDL_Event &event){
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return 1;

        switch (event.type) {
        case SDL_KEYDOWN:
            if (!keysToCommands.count(event.key.keysym.sym))
                continue;
            if (keysToCommands[event.key.keysym.sym] == Commands::QUIT)
                return 1;
            actionHandler(keysToCommands[event.key.keysym.sym], true);
            break;

        case SDL_KEYUP:
            if (!keysToCommands.count(event.key.keysym.sym))
                continue;
            actionHandler(keysToCommands[event.key.keysym.sym], false);
            break;

        case SDL_CONTROLLERAXISMOTION:
            continue;

        case SDL_CONTROLLERBUTTONDOWN:
            if (!buttonsToCommands.count(event.cbutton.button))
                continue;
            actionHandler(buttonsToCommands[event.cbutton.button], true);
            break;

        case SDL_CONTROLLERBUTTONUP:
            if (!buttonsToCommands.count(event.cbutton.button))
                continue;
            actionHandler(buttonsToCommands[event.cbutton.button], false);
            break;
        }
    }
    return 0;
}

bool EventHandler::isInitialized() const
{
    return initialized;
}

void EventHandler::executeEvents(){
    while (!eventStack.empty()) {
        DEBUG("Step2");
        // Check the message and see if there is anything that needs to be done.
        threadManager->sendMessage(buffers::input, std::make_unique<InputMessage>(std::move(eventStack.front())));
        eventStack.pop();
    }
}

void EventHandler::setPlayer(std::weak_ptr<phy::Body> bodyPtr){
    body = bodyPtr;
}

std::weak_ptr<const phy::Body> EventHandler::getPlayer() const
{
    return body;
}
