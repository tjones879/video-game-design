#pragma once

#include "inc/physics/common.hpp"
#include "inc/physics/body.hpp"
#include <memory>
#include <vector>

namespace phy {

class Body;
class Contact;

class World {
public:
    World(const Vec2 &gravity_);
    ~World();

    /**
     * Create a body using the given BodySpec.
     *
     * The given reference can be safely destroyed or reused after this function.
     * @return A pointer to the created body.
     */
    std::weak_ptr<Body> createBody(const BodySpec &spec);

    /**
     * Destroy the given body. Nothing occurs if the body can't be found.
     *
     * @param body A reference to the body that should be deleted.
     * @warning All associated shapes and joints are automatically destroyed.
     */
    void destroyBody(std::weak_ptr<Body> body);

    /**
     * Get all current bodies in the world.
     */
    std::vector<std::weak_ptr<const Body>> getBodies() const;

    /**
     * Obtain all current contacts created by the world.
     */
    std::vector<std::weak_ptr<const Contact>> getContacts() const;

    void step(float deltaTime);

    void setGravity(const Vec2 &gravity_);
    Vec2 getGravity() const;

    void setVelocityIterations(uint8_t iterations);
    void setPositionIterations(uint8_t iterations);
private:
    Vec2 gravity;
    std::vector<std::shared_ptr<Body>> bodyList;
    uint8_t velocityIterations;
    uint8_t positionIterations;
};
} /* namespace phy */
