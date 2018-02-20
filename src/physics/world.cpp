#include "inc/physics/world.hpp"
#include "inc/physics/body.hpp"

#include <algorithm>

namespace phy {

World::World(const Vec2 &gravity_)
    : gravity(gravity_)
{
}

World::~World()
{
    std::for_each(bodyList.begin(), bodyList.end(),
        [](auto &body) {
            // Destroy all of the body's shapes
        });
}

std::weak_ptr<Body> World::createBody(const BodySpec &spec)
{
    // TODO: Provide custom allocator to improve cache locality in vector and improve performance.
    // TODO: Synchronize to avoid data race.
    auto bodyPtr = std::make_shared<Body>(spec);
    bodyList.push_back(bodyPtr);
    return bodyPtr;
}

void World::destroyBody(std::weak_ptr<Body> body)
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

void World::step(float deltaTime)
{
    // If new bodies or shapes were added, find them
    // Lock the world
    float invDeltaTime = 0.0f;
    if (deltaTime > 0)
        invDeltaTime = 1.0f / deltaTime;

    // Update all contacts
    // Integrate velocities, solve velocity constraints, integrate positions
    // Handle TOI

}

void World::setGravity(const Vec2 &gravity_)
{
    gravity = gravity_;
}

Vec2 World::getGravity() const
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
} /* namespace phy */
