#include "inc/physics/world.hpp"
#include "inc/physics/body.hpp"
#include "SDL2/SDL.h"
#include "inc/threadmanager.hpp"

#include <algorithm>
#include <iostream>

namespace phy {

World::World(const Vec2f &gravity_, ThreadManager *manager)
    : gravity(gravity_), threadManager(manager), velocityIterations(10), positionIterations(10),
      lastTicks(SDL_GetTicks()) {}

World::~World() = default;

std::weak_ptr<Body> World::createBody(const BodySpec &spec)
{
    // TODO: Provide custom allocator to improve cache locality in vector and improve performance.
    auto bodyPtr = std::make_shared<Body>(spec);
    bodyList.push_back(bodyPtr);
    broadPhase.addNewBody(bodyPtr);
    for (auto shape : spec.shapes) {
        auto shapeType = shape->getShapeType();
        if (shapeType == ShapeType::circle) {
            bodyPtr->addShape(*std::move(std::dynamic_pointer_cast<CircleShape>(shape)));
        } else if (shapeType == ShapeType::polygon) {
            bodyPtr->addShape(*std::move(std::dynamic_pointer_cast<PolygonShape>(shape)));
        }
    }

    return bodyPtr;
}

void World::destroyBody(const std::weak_ptr<Body> &body)
{
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

std::unique_ptr<CollisionMessage> World::getCollisions()
{
    return std::make_unique<CollisionMessage>(broadPhase.getBodyCollisions());
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
    if (lastPause.first)
        return;

    const float dt = updateTime();

    // TODO: Update all contacts
    // Integrate velocities
    for (const auto body : bodyList)
        body->updateVelocity(dt, gravity);

    // TODO: Resolve velocity constraints
    // Integrate positions
    for (const auto body : bodyList) {
        if (!body->getExtraData()->colliding) {
            body->updatePosition(dt);
            broadPhase.updateBody(body);
        }
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
    using Polygons = RenderMessage::ShapeList<PolygonShape>;
    using Circles = RenderMessage::ShapeList<CircleShape>;
    Polygons polygons;
    Circles circles;

    for (auto&& b : bodyList) {
        for (auto&& s : b->shapeList) {
            if (s->getShapeType() == ShapeType::polygon) {
                auto polygon = std::dynamic_pointer_cast<phy::PolygonShape>(s);
                polygons.push_back(std::make_tuple(*polygon.get(),
                                                   b->getTransform(),
                                                   b->getExtraData()->color));
            } else {
                auto circle = std::dynamic_pointer_cast<phy::CircleShape>(s);
                circles.push_back(std::make_tuple(*circle.get(),
                                                  b->getTransform(),
                                                  b->getExtraData()->color));
            }
        }
    }

    auto msg = RenderMessage(std::make_unique<Polygons>(polygons),
                             std::make_unique<Circles>(circles));
    return std::make_unique<RenderMessage>(std::move(msg));
}

void World::pause()
{
    if (lastPause.first)
        return;

    lastPause = {true, SDL_GetTicks() - lastTicks};
    std::cout << "<" << lastPause.first << ", " << lastPause.second << ">" << std::endl;
}

void World::unpause()
{
    if (!lastPause.first)
        return;

    lastTicks = SDL_GetTicks() - lastPause.second;
    lastPause = {false, 0};
    std::cout << "<" << lastPause.first << ", " << lastPause.second << ">" << std::endl;
}
} /* namespace phy */
