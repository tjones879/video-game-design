#include <inc/displaymanager.hpp>
#include <iostream>
#include <cstdio>

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
    camera.zoom = .9;
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
    polygons.resize(msg->polygons->size());
    polygons = std::move(*msg->polygons.get());
    circles.resize(msg->circles->size());
    circles = std::move(*msg->circles.get());
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

    for (const auto &s : polygons) {
        auto shape = std::get<0>(s);
        auto transform = std::get<1>(s);

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
        //std::cout << shape << std::endl;
        auto vertices = toFloatVector(shape, transform);
        GPU_PolygonFilled(gpu, shape.vertices.size(), &vertices[0], std::get<2>(s));
    }

    for (const auto &s : circles) {
        auto shape = std::get<0>(s);
        auto transform = std::get<1>(s);
        auto center = transform.translate(shape.pos);
        GPU_CircleFilled(gpu, center.x, center.y, shape.radius, setPlayerColor());
        setCamera(center.x, center.y);
    }

    GPU_Flip(gpu);
}

int DisplayManager::getCamPosX() {
    return camera.x;
}

void DisplayManager::setCamera(const int playerPosX, const int playerPosY){
    //This tracks the camera to the player
    if(playerPosX < (camera.x + (SCREEN_WIDTH/2)-((SCREEN_WIDTH/2)/camera.zoom)*.75)){
        camera.x = (playerPosX - ((SCREEN_WIDTH/2)-((SCREEN_WIDTH/2)/camera.zoom)*.75));
    }
    if(playerPosX > (camera.x + (SCREEN_WIDTH/2)+((SCREEN_WIDTH/2)/camera.zoom)*.75)){
        camera.x = (playerPosX - ((SCREEN_WIDTH/2)+((SCREEN_WIDTH/2)/camera.zoom)*.75));
    }
    if(playerPosY < (camera.y + (SCREEN_HEIGHT/2)-((SCREEN_HEIGHT/2)/camera.zoom)*.75)){
        camera.y = (playerPosY - ((SCREEN_HEIGHT/2)-((SCREEN_HEIGHT/2)/camera.zoom)*.75));
    }
    if(playerPosY > (camera.y + (SCREEN_HEIGHT/2)+((SCREEN_HEIGHT/2)/camera.zoom)*.75)){
        camera.y = (playerPosY - ((SCREEN_HEIGHT/2)+((SCREEN_HEIGHT/2)/camera.zoom)*.75));
    }
    //This zooms the camera based on the enemy
    // if(camera.zoom <= 1.5 && camera.zoom >= 0.5){
    //     if(380 <= (camera.x + ((SCREEN_WIDTH/2)-((SCREEN_WIDTH/2)/camera.zoom)*.75))){
    //         if(camera.zoom >= 0.52)
    //             camera.zoom -= .005;
    //     }
    //     else if(420 >= (camera.x + ((SCREEN_WIDTH/2)+((SCREEN_WIDTH/2)/camera.zoom)*.75))){
    //         if(camera.zoom >= 0.52)
    //             camera.zoom -= .005;
    //     }
    //     else if(380 <= (camera.y + ((SCREEN_HEIGHT/2)-((SCREEN_HEIGHT/2)/camera.zoom)*.75))){
    //         if(camera.zoom >= 0.52)
    //             camera.zoom -= .005;
    //     }
    //     else if(420 >= (camera.y + ((SCREEN_HEIGHT/2)+((SCREEN_HEIGHT/2)/camera.zoom)*.75))){
    //         if(camera.zoom >= 0.52)
    //             camera.zoom -= .005;
    //     }
    //     else{
    //         if(camera.zoom <= 1.49)
    //             camera.zoom += .005;
    //     }
    // }
    GPU_SetCamera(gpu, &camera);
}

SDL_Color DisplayManager::setPlayerColor()
{

    SDL_Color tmpColor;

    if(colorAngle == 255 || colorAngle == 0)
        angleIncrement = !angleIncrement;

    if(angleIncrement){
        colorAngle += 1;
    } else colorAngle -= 1;
    
    tmpColor.r = (sin(colorAngle*M_PI/180)+1)*127.5;
    tmpColor.g = (sin(2*colorAngle*M_PI/180+3)+1)*127.5;
    tmpColor.b = (sin(1.5*colorAngle*M_PI/180+2)+1)*127.5;
    return tmpColor;
}