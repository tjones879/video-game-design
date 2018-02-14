#include "inc/physics/body.hpp"

namespace phy {
Body::Body(BodySpec &spec)
{

}

Body::~Body()
{

}

std::weak_ptr<Shape> Body::createShape(const ShapeSpec &spec)
{
    return std::make_shared<Shape>();
}

void Body::destroyShape(std::weak_ptr<Shape> shape)
{

}

const Vec2& Body::getPosition() const
{
    return position;
}

float Body::getRotation() const
{

}

void Body::setLinearVelocity(const Vec2 &velocity)
{

}

const Vec2 &Body::getLinearVelocity() const
{
    return linearVelocity;
}

void Body::setAngularVelocity(const Vec2 &velocity)
{

}

const Vec2 &Body::getAngularVelocity()
{
    return angularVelocity;
}

void Body::applyForce(const Vec2 &force, const Vec2 &point)
{

}

void Body::applyTorque(float torque)
{

}

void Body::applyLinearImpulse(const Vec2 &impulse, const Vec2 &point)
{

}

void Body::applyAngularImpulse(float impulse)
{

}

float Body::getMass() const
{
    return 0.0f;
}

float Body::getInertia() const
{
    return 0.0f;
}

const Vec2 &Body::getCenterMass() const
{
    return centroid;
}

void Body::setSleep()
{

}

bool Body::isAsleep()
{
    return true;
}

std::vector<std::weak_ptr<Shape>> Body::getShapes()
{
    return std::vector<std::weak_ptr<Shape>>(shapeList.begin(), shapeList.end());
}

std::vector<std::weak_ptr<const Shape>> Body::getShapes() const
{
    return std::vector<std::weak_ptr<const Shape>>(shapeList.begin(), shapeList.end());
}

std::vector<std::weak_ptr<Joint>> Body::getJoints()
{
    return std::vector<std::weak_ptr<Joint>>(jointList.begin(), jointList.end());
}

std::vector<std::weak_ptr<const Joint>> Body::getJoints() const
{
    return std::vector<std::weak_ptr<const Joint>>(jointList.begin(), jointList.end());
}

std::weak_ptr<World> Body::getParentWorld()
{
    return parentWorld;
}

std::weak_ptr<const World> Body::getParentWorld() const
{
    return parentWorld;
}
} /* namespace phy */
