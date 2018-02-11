#pragma once

#include "inc/physics/common.hpp"
#include "inc/physics/joint.hpp"
#include "inc/physics/body.hpp"
#include <memory>
#include <vector>

namespace phy {

class Body;
class Contact;
class Joint;

class World {
public:
    World(const Vec2 &gravity_);
    ~World();

    std::weak_ptr<Body> createBody();
    void destroyBody();
    std::vector<std::weak_ptr<const Body>> getBodies() const;

    std::weak_ptr<Joint> createJoint();
    void destroyJoint();
    std::vector<std::weak_ptr<const Joint>> getJoints() const;

    std::vector<std::weak_ptr<const Contact>> getContacts() const;

    void step(float deltaTime);

    void setGravity(const Vec2 &gravity_);
    Vec2 getGravity() const;

    void setVelocityIterations(uint8_t iterations);
    void setPositionIterations(uint8_t iterations);
private:
    Vec2 gravity;
    std::vector<Body> bodyList;
    std::vector<Joint> jointList;
    uint8_t velocityIterations;
    uint8_t positionIterations;
};
} /* namespace phy */
