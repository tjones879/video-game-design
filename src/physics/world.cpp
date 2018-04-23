#include "inc/physics/world.hpp"
#include "inc/physics/body.hpp"
#include "SDL2/SDL.h"

#include <algorithm>
#include <iostream>

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
    broadPhase.addNewBody(bodyPtr);
    return bodyPtr;
}

void World::destroyBody(const std::weak_ptr<Body> &body)
{
    // TODO: Synchronize to avoid data race
    auto result = std::find(std::begin(bodyList), std::end(bodyList), body.lock());
    if (result != std::end(bodyList)) {
        broadPhase.deleteBody(body.lock());
        bodyList.erase(result);
    }
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
    for (const auto body : bodyList)
        body->updateVelocity(dt, gravity);

    // TODO: Resolve velocity constraints
    // Integrate positions
    for (const auto body : bodyList) {
       body->updatePosition(dt);
       broadPhase.updateBody(body);
    }

    // TODO: Resolve position constraints
    broadPhase.updatePairs();

    // TODO: Synchronize shapes for broad-phase
    // TODO: Handle TOI

    // Clear forces
    for (const auto &body : bodyList)
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

std::unique_ptr<RenderMessage> World::getObjects()
{
    RenderMessage::ShapeList shapes;

    for (auto&& b : bodyList) {
        for (auto&& s : b->shapeList) {
            auto polygon = std::dynamic_pointer_cast<phy::PolygonShape>(s);
            shapes.push_back(std::make_pair(*polygon.get(), b->getTransform()));
        }
    }

    auto msg = RenderMessage(std::make_unique<RenderMessage::ShapeList>(shapes));
    return std::make_unique<RenderMessage>(std::move(msg));
}
} /* namespace phy */
