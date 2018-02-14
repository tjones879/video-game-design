#include "inc/physics/body.hpp"
#include "inc/physics/circle.hpp"
#include <algorithm>

namespace phy {
Body::Body(const BodySpec &spec)
{
    bodySweep.center0 = spec.position;
    bodySweep.center = spec.position;
    bodySweep.angle = spec.angle;
    bodySweep.angle0 = spec.angle;

    linearVelocity = spec.linVelocity;
    angularVelocity = spec.angVelocity;
    gravityFactor = spec.gravityFactor;
    bodyType = spec.bodyType;
    if (bodyType == BodyType::staticBody) {
        mass = 0.0f;
        invMass = 0.0f;
    } else {
        mass = 1.0f;
        invMass = 1.0f;
    }

    inertia = 0.0f;
    invInertia = 0.0f;
    torque = 0.0f;
}

Body::~Body()
{

}

std::weak_ptr<Shape> Body::createShape(const ShapeSpec &spec)
{
    return std::make_shared<CircleShape>();
}

void Body::destroyShape(std::weak_ptr<Shape> shape)
{
    auto result = std::find(std::begin(shapeList), std::end(shapeList), shape.lock());
    if (result != std::end(shapeList))
        shapeList.erase(result);
}

const Vec2& Body::getPosition() const
{
    return position;
}

float Body::getRotation() const
{
    return bodySweep.angle;
}

void Body::setLinearVelocity(const Vec2 &velocity)
{
    linearVelocity = velocity;
}

const Vec2 &Body::getLinearVelocity() const
{
    return linearVelocity;
}

void Body::setAngularVelocity(float velocity)
{
    angularVelocity = velocity;
}

float Body::getAngularVelocity()
{
    return angularVelocity;
}

void Body::applyForce(const Vec2 &force_, const Vec2 &point)
{
    force += force_;
    torque += (point - bodySweep.center).cross(force);
}

void Body::applyTorque(float torque_)
{
    torque += torque_;
}

void Body::applyLinearImpulse(const Vec2 &impulse, const Vec2 &point)
{
    if (bodyType != BodyType::dynamicBody)
        return

    linearVelocity += impulse;
    angularVelocity += (point - bodySweep.center).cross(impulse);
}

void Body::applyAngularImpulse(float impulse)
{
    angularVelocity += invInertia * impulse;
}

float Body::getMass() const
{
    return mass;
}

float Body::getInertia() const
{
    return inertia;
}

const Vec2 &Body::getCenterMass() const
{
    return centroid;
}

void Body::setSleep()
{
    return;
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

std::weak_ptr<World> Body::getParentWorld()
{
    return parentWorld;
}

std::weak_ptr<const World> Body::getParentWorld() const
{
    return parentWorld;
}
} /* namespace phy */
