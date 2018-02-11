#include "inc/physics/world.hpp"
#include "inc/physics/body.hpp"

namespace phy {

World::World(const Vec2 &gravity_)
    : gravity(gravity_)
{

}

World::~World()
{

}

std::weak_ptr<Body> World::createBody()
{
    return std::make_shared<Body>(Body());
}

void World::destroyBody()
{

}

std::vector<std::weak_ptr<const Body>> World::getBodies() const
{
    return std::vector<std::weak_ptr<const Body>>();
}

std::weak_ptr<Joint> createJoint()
{
    return std::make_shared<Joint>(Joint());
}

void World::destroyJoint()
{

}

std::vector<std::weak_ptr<const Joint>> World::getJoints() const
{
    return std::vector<std::weak_ptr<const Joint>>();
}

std::vector<std::weak_ptr<const Contact>> World::getContacts() const
{
    return std::vector<std::weak_ptr<const Contact>>();
}

void World::step(float deltaTime)
{

}
/*
class World {
public:
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
private:
    Vec2 gravity;
    std::vector<Body> bodyList;
    std::vector<Joint> jointList;
    uint8_t velocityIterations;
    uint8_t positionIterations;
};
 */

} /* namespace phy */
