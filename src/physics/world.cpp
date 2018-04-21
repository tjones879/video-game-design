#include "inc/physics/world.hpp"
#include "inc/physics/body.hpp"
#include "SDL2/SDL.h"

#include <algorithm>

namespace phy {

World::World(const Vec2f &gravity_)
    : gravity(gravity_), velocityIterations(10), positionIterations(10),
      lastTicks(SDL_GetTicks()) {}

World::~World() = default;

std::weak_ptr<Body> World::createBody(const BodySpec &spec)
{
    // TODO: Provide custom allocator to improve cache locality in vector and improve performance.
    // TODO: Synchronize to avoid data race.
    auto bodyPtr = std::make_shared<Body>(spec);
    bodyList.push_back(bodyPtr);
    return bodyPtr;
}

void World::destroyBody(const std::weak_ptr<Body> &body)
{
    // TODO: Synchronize to avoid data race
    auto result = std::find(std::begin(bodyList), std::end(bodyList), body.lock());
    if (result != std::end(bodyList))
        bodyList.erase(result);
}

std::vector<std::weak_ptr<const Body>> World::getBodies() const
{
    return std::vector<std::weak_ptr<const Body>>(bodyList.begin(), bodyList.end());
}

std::vector<std::weak_ptr<const Contact>> World::getContacts() const
{
    return std::vector<std::weak_ptr<const Contact>>();
}

float World::updateTime()
{
    uint32_t currentTicks = SDL_GetTicks();
    float dt = (currentTicks - lastTicks) / 1000.f;
    lastTicks = currentTicks;
    return dt;
}

void World::step()
{
    const float dt = updateTime();
    // If new bodies or shapes were added, find them
    // Lock the world

    // TODO: Update all contacts
    // Integrate velocities
    for (auto body : bodyList)
        body->updateVelocity(dt, gravity);

    // TODO: Resolve velocity constraints
    // Integrate positions
    for (auto body : bodyList)
       body->updatePosition(dt);

    // TODO: Resolve position constraints

    // TODO: Synchronize shapes for broad-phase
    // TODO: Handle TOI

    // Clear forces
    for (auto body : bodyList)
       body->clearForces();
}

void World::setGravity(const Vec2f &gravity_)
{
    gravity = gravity_;
}

Vec2f World::getGravity() const
{
    return gravity;
}

void World::setVelocityIterations(uint8_t iterations)
{
    velocityIterations = iterations;
}

void World::setPositionIterations(uint8_t iterations)
{
    positionIterations = iterations;
}

RenderMessage World::getObjects()
{
    std::vector<std::pair<phy::PolygonShape, phy::Transform>> shapes;

    for (auto&& b : bodyList) {
        for (auto&& s : b->shapeList) {
            auto polygon = std::dynamic_pointer_cast<std::shared_ptr<phy::PolygonShape>>(s);
            shapes.push_back(std::make_pair(*s, /* TODO */))
        }
    }
}
} /* namespace phy */
