#pragma once

#include "inc/physics/common.hpp"
#include "inc/physics/body.hpp"
#include <memory>
#include <vector>

namespace phy {

class Body;
class Contact;

class World {
private:
    Vec2 gravity;
    std::vector<std::shared_ptr<Body>> bodyList;
    uint8_t velocityIterations;
    uint8_t positionIterations;
public:
    World(const Vec2 &gravity_);
    /**
     * All objects referenced by the world are ref counted, so
     * they should be automatically destroyed when any other
     * strong references are removed.
     */
    ~World();

    /**
     * Create a body using the given BodySpec.
     *
     * The given reference can be safely destroyed or reused after this function.
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

    void step(float dt);

    void setGravity(const Vec2 &gravity_);
    Vec2 getGravity() const;

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
};
} /* namespace phy */
