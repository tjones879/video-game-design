#include <inc/messagetypes.hpp>
#include <inc/eventhandler.hpp>
#include <inc/sound.hpp>
#include <iostream>
#include <cstdio>
#include <random>

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

int EventHandler::getSoundOrigin() {
    auto playerPos = player.lock()->getPosition();
    double soundRatio = double(abs(camPosX - playerPos.x))/640; //640 is Screen width
    int soundOrigin = soundRatio*255;// 255 is max volume per channel
    return soundOrigin*-1; // return inverse since this is used for left channel -- see Mix_SetPanning in sound.cpp
}


void EventHandler::setCamPosX(int camX) {
    camPosX = camX;
}

void EventHandler::actionHandler(Commands command, bool pressed)
{
    std::unique_ptr<Command> cmd;
    if (pressed && !commandState[static_cast<char>(command)]) {
        switch (command) {
        case Commands::JUMP:
            DEBUG("Jump");
            cmd = std::make_unique<MoveCommand>(player, Vec2<int>( 0,-5));
            break;
        case Commands::DUCK:
            DEBUG("Duck");
            cmd = std::make_unique<MoveCommand>(player, Vec2<int>( 0, 5));
            break;
        case Commands::BACK:
            DEBUG("Back");
            cmd = std::make_unique<MoveCommand>(player, Vec2<int>(-5, 0));
            break;
        case Commands::FORWARD:
            DEBUG("Forward");
            cmd = std::make_unique<MoveCommand>(player, Vec2<int>( 5, 0));
            break;
        case Commands::ACTION:
            DEBUG("Action");
            cmd = std::make_unique<ActionCommand>(player);
            break;
        case Commands::SPECIAL:
            DEBUG("Special");
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
        if (eventStack.front()->type == Commands::ACTION)
            threadManager->sendMessage(buffers::sound,
                                       std::make_unique<AudioMessage>(getSoundOrigin()));
        threadManager->sendMessage(buffers::input,
                                   std::make_unique<InputMessage>(std::move(eventStack.front())));
        eventStack.pop();
    }
}

void EventHandler::setPlayer(std::weak_ptr<phy::Body> bodyPtr){
    player = bodyPtr;
}

std::weak_ptr<const phy::Body> EventHandler::getPlayer() const
{
    return player;
}

std::vector<phy::BodySpec> EventHandler::defineEnemies(uint32_t numEnemies) const
{
    // Initialize randomness
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> vel(-50, 50);
    std::uniform_real_distribution<float> pos(200, 300);
    std::uniform_int_distribution<uint8_t> color(0, 255);

    std::vector<phy::BodySpec> specs;
    for (int i = 0; i < numEnemies; i++) {
        phy::BodySpec spec;
        spec.bodyType = phy::BodyType::dynamicBody;
        spec.position = {pos(gen), pos(gen)};
        spec.gravityFactor = 0;
        auto shape = phy::PolygonShape(1.0f);
        shape.setBox(Vec2<float>(5, 5));
        spec.shapes.push_back(std::make_shared<phy::PolygonShape>(shape));

        spec.linVelocity = {vel(gen), vel(gen)};
        // TODO: Give enemy random color
        SDL_Color c{color(gen), color(gen), color(gen), color(gen)};
        spec.extra.color = c;
        specs.push_back(spec);
    }
    return specs;
}

std::unordered_set<std::shared_ptr<phy::Body>> EventHandler::getEnemies() const
{
    return enemies;
}

void EventHandler::enemyMovement() {
    int dirx, diry;
    if (!player.lock())
        return;
    auto playerPos = player.lock()->getPosition();

    for (auto enemy:enemies) {
        auto enemyPos = enemy->getPosition();

        dirx = playerPos.x - enemyPos.x;
        diry = playerPos.y - enemyPos.y;

        if (dirx > 0 && diry > 0) {
            enemy->setLinearVelocity(Vec2<float>(20,20));
        }
        if (dirx > 0 && diry < 0) {
            enemy->setLinearVelocity(Vec2<float>(20,-20));
        }
        if (dirx < 0 && diry > 0) {
            enemy->setLinearVelocity(Vec2<float>(-20,20));
        }
        if (dirx < 0 && diry < 0) {
            enemy->setLinearVelocity(Vec2<float>(-20,-20));
        }


    }
}

void EventHandler::addEnemy(std::shared_ptr<phy::Body> enemy)
{
    enemies.insert(enemy);
}
