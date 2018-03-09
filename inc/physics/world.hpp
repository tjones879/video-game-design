#pragma once

#include "inc/physics/common.hpp"
#include "inc/physics/body.hpp"
#include <memory>
#include <vector>

namespace phy {

class Body;
class Contact;

/**
 * The world contains all bodies and constraints in the world.
 *
 * This class should be the primary entry point of any interactions
 * with the physics engine.
 *
 * New bodies should be instantiated using a BodySpec and passed
 * to World::createBody(spec). This body can be moved manually
 * (in the case of a player-controlled character), but all collisions
 * will be notified through a collision listener.
 */
class World {
private:
    Vec2f gravity;
    std::vector<std::shared_ptr<Body>> bodyList;
    uint8_t velocityIterations; ///< Number of iterations used to resolve velocity of bodies
    uint8_t positionIterations; ///< Number of iterations used to resolve positions of bodies
    uint32_t lastTicks; ///< Number of SDL_GetTicks() for the last iteration
public:
    World(const Vec2f &gravity_);
    /**
     * All objects referenced by the world are ref counted, so
     * they should be automatically destroyed when any other
     * strong references are removed.
     */
    ~World();

    /**
     * Create a body using the given BodySpec.
     *
     * @return A pointer to the created body.
     */
    std::weak_ptr<Body> createBody(const BodySpec &spec);

    /**
     * Remove the reference to the given body.
     *
     * Nothing occurs if the body can't be found.
     *
     * Note that the body will not be destroyed until all other strong
     * references are also destroyed.
     *
     * @param body A reference to the body that should be deleted.
     */
    void destroyBody(const std::weak_ptr<Body> &body);

    /**
     * Get all current bodies in the world.
     */
    std::vector<std::weak_ptr<const Body>> getBodies() const;

    /**
     * Obtain all current contacts created by the world.
     */
    std::vector<std::weak_ptr<const Contact>> getContacts() const;

    void step();

    void setGravity(const Vec2f &gravity_);
    Vec2f getGravity() const;

    /**
     * Set the number of iterations the world should use to resolve
     * the velocity of bodies.
     *
     * More iterations will reduce performance but increase accuracy
     * and prevent impossible conditions.
     */
    void setVelocityIterations(uint8_t iterations);

    /**
     * Set the number of iterations the world should use to resolve
     * the global position of bodies.
     *
     * More iterations will reduce performance but increase accuracy
     * and prevent impossible conditions.
     */
    void setPositionIterations(uint8_t iterations);
private:
    float updateTime();
};
} /* namespace phy */
