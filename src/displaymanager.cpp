#include <inc/displaymanager.hpp>
#include <iostream>
#include <cstdio>

#define DEBUG(e) std::cerr << e << std::endl;

DisplayManager::DisplayManager(const std::string &title)
    : initialized(false), window(nullptr), gpu(&window)
{
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
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
    // camera.x = 150 - (SCREEN_HEIGHT/2);
    // camera.y = 150 - (SCREEN_HEIGHT/2);
    camera.x = 0;
    camera.y = 0;
    camera.zoom = 1.5;
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

void DisplayManager::displayPolygon(const std::vector<std::weak_ptr<phy::Body>> &bodies,
                                    const std::vector<std::weak_ptr<phy::PolygonShape>> &shapes)
{
    GPU_Clear(gpu);
    color.r = 0;
    color.g = 125;
    color.b = 125;
    color.a = 255;


    for (int i = 0; i < bodies.size(); i++) {
        std::weak_ptr<phy::Body> body = bodies[i];
        std::weak_ptr<phy::PolygonShape> shape = shapes[i];
        auto offset = body.lock()->getPosition();
        auto vel = body.lock()->getLinearVelocity();
        //std::cout << offset.x << ' ' << i << std::endl;
        setCamera(bodies[0].lock()->getLinearVelocity(),bodies[0].lock()->getPosition(),bodies[1].lock()->getLinearVelocity(),bodies[1].lock()->getPosition());

        std::vector<float> vertices;
        vertices.reserve(shape.lock()->vertices.size() * 2);
        for (auto v : shape.lock()->vertices) {
            vertices.push_back(v.x + offset.x);
            vertices.push_back(v.y + offset.y);
        }
        if(i==0){
            setPlayerColor();
            GPU_PolygonFilled(gpu, shape.lock()->vertices.size(), &vertices[0], playerColor);
        }else
            GPU_PolygonFilled(gpu, shape.lock()->vertices.size(), &vertices[0], color);
    }
    GPU_Flip(gpu);
}

void DisplayManager::setCamera(const Vec2<float> playerVel, 
                               const Vec2<float> playerPos, 
                               const Vec2<float> enemyVel, 
                               const Vec2<float> enemyPos){
    //This tracks the camera to the player
    std::cout << "Cam x,y: " << camera.x << "," << camera.y << " CamZoom: " << camera.zoom << " Plyr x,y: " << playerPos.x << "," << playerPos.y << std::endl;
    // if(playerPos.x < (camera.x + ((2-camera.zoom)*SCREEN_WIDTH/4)))
    //     camera.x = (playerPos.x - ((2-camera.zoom)*SCREEN_WIDTH/4));
    // if(playerPos.x > (camera.x + (.75*(2-camera.zoom)*SCREEN_WIDTH)))
    //     camera.x = (playerPos.x - (.75*(2-camera.zoom)*SCREEN_WIDTH));
    // if(playerPos.y < (camera.y + ((2-camera.zoom)*SCREEN_HEIGHT/4)))
    //     camera.y = (playerPos.y - ((2-camera.zoom)*SCREEN_HEIGHT/4));
    // if(playerPos.y > (camera.y + (3*(2-camera.zoom)*SCREEN_HEIGHT/4)))
    //     camera.y = (playerPos.y - (3*(2-camera.zoom)*SCREEN_HEIGHT/4));
    //This zooms the camera based on the enemy
    // if(camera.zoom <= 1.5 && camera.zoom >= 0.5){
    //     if(enemyPos.x < (camera.x + (SCREEN_WIDTH/4))){
    //         if(camera.zoom >= 0.52)
    //             camera.zoom -= .01;
    //     }
    //     else if(enemyPos.x > (camera.x + (3*SCREEN_WIDTH/4))){
    //         if(camera.zoom >= 0.52)
    //             camera.zoom -= .01;
    //     }
    //     else if(enemyPos.y < (camera.y + (SCREEN_HEIGHT/4))){
    //         if(camera.zoom >= 0.52)
    //             camera.zoom -= .01;
    //     }
    //     else if(enemyPos.y > (camera.y + (3*SCREEN_HEIGHT/4))){
    //         if(camera.zoom >= 0.52)
    //             camera.zoom -= .01;
    //     }
    //     else{
    //         if(camera.zoom <= 1.49)
    //             camera.zoom += .01;
    //     }
    // }
    GPU_SetCamera(gpu, &camera);
}

void DisplayManager::setPlayerColor(){
    if ( playerColor.r == 255 || playerColor.r == 0)
        r = !r;
    if ( playerColor.g == 254 || playerColor.g == 0)
        g = !g;
    if ( playerColor.b == 255 || playerColor.b == 0)
        b = !b;
    if ( !r )
        playerColor.r += 1;
    if ( !g )
        playerColor.g += 2;
    if ( !b )
        playerColor.b += 3;
    if ( r )
        playerColor.r -= 1;
    if ( g )
        playerColor.g -= 2;
    if ( b )
        playerColor.b -= 3;
}