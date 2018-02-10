#pragma once

#include <inc/physics/common.hpp>

namespace phy {

class Body;
class Contact;
class Joint;

class World {
public:
    World(const Vec2 &gravity_);
    ~World();

    Body *createBody();
    void destroyBody();
    const Body *getBodies() const;

    Joint *createJoint();
    void destroyJoint();
    const Joint *getJoints() const;

    const Contact *getContacts() const;

    void step(float deltaTime);

    void setGravity(const Vec2 &gravity_);
    Vec2 getGravity() const;

    void setVelocityIterations(uint8_t iterations);
    void setPositionIterations(uint8_t iterations);
private:
    Vec2 gravity;
    Body *bodyList;
    uint8_t velocityIterations;
    uint8_t positionIterations;
};
} /* namespace phy */
