#include <inc/displaymanager.hpp>
#include <iostream>
#include <cstdio>

#define DEBUG(e) std::cerr << e << std::endl;

DisplayManager::DisplayManager(const std::string &title)
    : initialized(false), window(nullptr), gpu(&window)
{
    if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return;
    }

    if (!gpu.screen) {
        std::cout << "sdl_gpu failed to initialize." << std::endl;
        return;
    } else {
        SDL_SetWindowTitle(window, title.c_str());
        initialized = true;
    }
    playerColor.r = 1;
    playerColor.g = 2;
    playerColor.b = 3;
    playerColor.a = 255;
    camera = GPU_GetCamera(gpu);
    GPU_EnableCamera(gpu, true);
    camera.x = 0;
    camera.y = 0;
    camera.zoom = .5;
    GPU_SetCamera(gpu, &camera);
}

DisplayManager::~DisplayManager()
{
    SDL_DestroyWindow(window);
    window = nullptr;
}

DisplayManager::operator SDL_Window*() const
{
    return window;
}

bool DisplayManager::isInitialized() const
{
    return initialized;
}

void DisplayManager::addRenderable(std::unique_ptr<RenderMessage>&& msg)
{
    shapes.resize(msg->shapes->size());
    shapes = std::move(*msg->shapes.get());
}

inline std::vector<float> DisplayManager::toFloatVector(const phy::PolygonShape &shape,
                                                        const phy::Transform &offset)
{
    std::vector<float> vertices;
    vertices.reserve(shape.vertices.size() * 2);
    for (auto v : shape.vertices) {
        auto point = std::move(offset.translate(v));
        vertices.push_back(point.x);
        vertices.push_back(point.y);
    }

    return vertices;
}

void DisplayManager::displayAll()
{
    GPU_Clear(gpu);
    color.r = 0;
    color.g = 125;
    color.b = 125;
    color.a = 255;

    for (const auto &s : shapes) {
        auto shape = s.first;
        auto transform = s.second;

    /*
    for (int i = 0; i < bodies.size(); i++) {
        std::weak_ptr<phy::Body> body = bodies[i];
        std::weak_ptr<phy::PolygonShape> shape = shapes[i];
        auto offset = body.lock()->getPosition();
        auto vel = body.lock()->getLinearVelocity();
        setCamera(bodies[0].lock()->getLinearVelocity(),
                  bodies[0].lock()->getPosition(),
                  bodies[1].lock()->getLinearVelocity(),
                  bodies[1].lock()->getPosition());

        if(!i == 0){
            setPlayerColor();
            GPU_PolygonFilled(gpu, shape.lock()->vertices.size(), &vertices[0], playerColor);
        }else
            GPU_PolygonFilled(gpu, shape.lock()->vertices.size(), &vertices[0], color);
        */
        auto vertices = std::move(toFloatVector(shape, transform));
        GPU_PolygonFilled(gpu, shape.vertices.size(), &vertices[0], color);
    }

    GPU_Flip(gpu);
}

int DisplayManager::getCamPosX() {
    return camera.x;
}

void DisplayManager::setCamera(const Vec2<float> playerVel,
                               const Vec2<float> playerPos,
                               const Vec2<float> enemyVel,
                               const Vec2<float> enemyPos){
    //This tracks the camera to the player
    if(playerPos.x < (camera.x + (SCREEN_WIDTH/2)-((SCREEN_WIDTH/2)/camera.zoom)*.75)){
        camera.x = (playerPos.x - ((SCREEN_WIDTH/2)-((SCREEN_WIDTH/2)/camera.zoom)*.75));
    }
    if(playerPos.x > (camera.x + (SCREEN_WIDTH/2)+((SCREEN_WIDTH/2)/camera.zoom)*.75)){
        camera.x = (playerPos.x - ((SCREEN_WIDTH/2)+((SCREEN_WIDTH/2)/camera.zoom)*.75));
    }
    if(playerPos.y < (camera.y + (SCREEN_HEIGHT/2)-((SCREEN_HEIGHT/2)/camera.zoom)*.75)){
        camera.y = (playerPos.y - ((SCREEN_HEIGHT/2)-((SCREEN_HEIGHT/2)/camera.zoom)*.75));
    }
    if(playerPos.y > (camera.y + (SCREEN_HEIGHT/2)+((SCREEN_HEIGHT/2)/camera.zoom)*.75)){
        camera.y = (playerPos.y - ((SCREEN_HEIGHT/2)+((SCREEN_HEIGHT/2)/camera.zoom)*.75));
    }
    //This zooms the camera based on the enemy
    if(camera.zoom <= 1.5 && camera.zoom >= 0.5){
        if(enemyPos.x <= (camera.x + ((SCREEN_WIDTH/2)-((SCREEN_WIDTH/2)/camera.zoom)*.75))){
            if(camera.zoom >= 0.52)
                camera.zoom -= .005;
        }
        else if(enemyPos.x >= (camera.x + ((SCREEN_WIDTH/2)+((SCREEN_WIDTH/2)/camera.zoom)*.75))){
            if(camera.zoom >= 0.52)
                camera.zoom -= .005;
        }
        else if(enemyPos.y <= (camera.y + ((SCREEN_HEIGHT/2)-((SCREEN_HEIGHT/2)/camera.zoom)*.75))){
            if(camera.zoom >= 0.52)
                camera.zoom -= .005;
        }
        else if(enemyPos.y >= (camera.y + ((SCREEN_HEIGHT/2)+((SCREEN_HEIGHT/2)/camera.zoom)*.75))){
            if(camera.zoom >= 0.52)
                camera.zoom -= .005;
        }
        else{
            if(camera.zoom <= 1.49)
                camera.zoom += .005;
        }
    }
    GPU_SetCamera(gpu, &camera);
}

void DisplayManager::setPlayerColor(){
    if (playerColor.r == 255 || playerColor.r == 0)
        r = !r;
    if (playerColor.g == 254 || playerColor.g == 0)
        g = !g;
    if (playerColor.b == 255 || playerColor.b == 0)
        b = !b;
    if (!r)
        playerColor.r += 1;
    if (!g)
        playerColor.g += 2;
    if (!b)
        playerColor.b += 3;
    if (r)
        playerColor.r -= 1;
    if (g)
        playerColor.g -= 2;
    if (b)
        playerColor.b -= 3;
}
