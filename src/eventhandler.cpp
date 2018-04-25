#include <inc/eventhandler.hpp>
#include <inc/sound.hpp>
#include <iostream>
#include <cstdio>
#define WAV_PATH "assets/high.wav"

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

EventHandler::EventHandler(){
    initialized = true;
    commandState.fill(false);
    initKeyMapping();
    initButtonMapping();
    duckCommand = new DuckCommand;
    specialCommand = new SpecialCommand;
    actionCommand = new ActionCommand;
    moveCommand = new MoveCommand(this);
    jumpCommand = new JumpCommand;


    // Temporarily open the first joystick to the controller if it exists
    if (SDL_NumJoysticks() > 0) {
        controller.setJoystick(0);
        std::cout << controller << std::endl;
    }
}

EventHandler::~EventHandler(){
    delete jumpCommand;
    delete moveCommand;
    delete actionCommand;
    delete specialCommand;
    delete duckCommand;
}

int EventHandler::getSoundOrigin() {
    auto player = body.lock()->getPosition();
    DEBUG(camPosX);
    double soundRatio = double(abs(camPosX - player.x))/640; //640 is Screen width
    int soundOrigin = soundRatio*255;// 255 is max volume per channel
    DEBUG(soundOrigin);
    return soundOrigin*-1; // return inverse since this is used for left channel -- see Mix_SetPanning in sound.cpp
}

void EventHandler::setCamPosX(int camX) {
    camPosX = camX;
}

void EventHandler::actionHandler(Commands command, bool pressed)
{
    Sound* effect = new Sound(WAV_PATH, SOUND_EFFECT);
    if (pressed && !commandState[static_cast<char>(command)]) {
        switch (command) {
        case Commands::JUMP:
            DEBUG("Jump");
            moveCommand->addCommand({0,-6});
            break;
        case Commands::DUCK:
            DEBUG("Duck");
            moveCommand->addCommand({0,6});
            break;
        case Commands::BACK:
            DEBUG("Back");
            moveCommand->addCommand({-6,0});
            break;
        case Commands::FORWARD:
            DEBUG("Forward");
            moveCommand->addCommand({6,0});
            break;
        case Commands::ACTION:
            DEBUG("Action");
            effect->playSound(getSoundOrigin());
            eventStack.push(actionCommand);
            break;
        case Commands::SPECIAL:
            DEBUG("Special");
            eventStack.push(specialCommand);
            break;
        default:
            break;
        }
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

void EventHandler::addEvent(Command &newCommand){
    eventStack.push(&newCommand);
}

void EventHandler::executeEvents(){
    while (!eventStack.empty()) {
        eventStack.front()->execute();
        eventStack.pop();
    }
}

auto EventHandler::getCommandPtr(Commands cmd) -> Command*{
    switch (cmd) {
        case Commands::JUMP:
            return jumpCommand;
        case Commands::DUCK:
            return duckCommand;
        case Commands::BACK:
            addPlayerVel({-1,0});
            return moveCommand;
        case Commands::FORWARD:
            addPlayerVel({1,0});
            return moveCommand;
        case Commands::ACTION:
            return actionCommand;
        case Commands::SPECIAL:
            return actionCommand;
        default:
            break;
    }
}

void EventHandler::setPlayer(std::weak_ptr<phy::Body> bodyPtr){
    /*
    body
    auto currVel = body->getLinearVelocity();
    body->setLinearVelocity(currVel + {5, 0});
    */
    body = bodyPtr;
}

void EventHandler::addPlayerVel(Vec2<int> addVelocity){
    auto currVel = body.lock()->getLinearVelocity();
    body.lock()->setLinearVelocity(currVel + addVelocity);
}
