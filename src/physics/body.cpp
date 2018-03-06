#include "inc/physics/body.hpp"
#include <algorithm>

namespace phy {
Body::Body(const BodySpec &spec)
    : extraData(nullptr)
{
    bodySweep.center0 = spec.position;
    bodySweep.center = spec.position;
    bodySweep.angle = spec.angle;
    bodySweep.angle0 = spec.angle;

    angle = spec.angle;

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

Body::~Body() = default;

std::weak_ptr<PolygonShape> Body::addShape(const PolygonShape &shape)
{
    auto ptr = std::make_shared<PolygonShape>(shape);
    shapeList.push_back(ptr);
    updateMassProperties();
    return ptr;
}

std::weak_ptr<CircleShape> Body::addShape(const CircleShape &shape)
{
    auto ptr = std::make_shared<CircleShape>(shape);
    shapeList.push_back(ptr);
    updateMassProperties();
    return ptr;
}

void Body::destroyShape(const std::weak_ptr<Shape> &shape)
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

void Body::updateVelocity(float dt, Vec2 gravity)
{
    if (bodyType == BodyType::dynamicBody) {
        linearVelocity += (gravity * gravityFactor + force * invMass) * dt;
        angularVelocity = invInertia * torque * dt;
        // TODO: Apply damping
    }
}

void Body::updatePosition(float dt)
{
    Vec2 translation = linearVelocity * dt;
    float rotation = angularVelocity * dt;
    position += translation;
    angle += rotation;
}

void Body::clearForces()
{
    force.zeroOut();
    torque = 0.0f;
}

void Body::updateMassProperties()
{
    mass = 0.0f;
    invMass = 0.0f;
    inertia = 0.0f;
    invInertia = 0.0f;
    bodySweep.center.zeroOut();

    Vec2 localCenter;
    // Sum the mass of all shapes
    // Calculate the center of mass
    // Center the inertia
    // Update velocity
}

void *Body::getExtraData() const
{
    return extraData;
}

void Body::setExtraData(void *data)
{
    extraData = data;
}

std::vector<Transform> Body::getShapePositions() const
{
    transforms.reserve(shapeList.size());
    for (const auto &shape : shapeList) {
        transforms.emplace_back(std::make_pair(shape, ));
    }
}

std::ostream& operator<<(std::ostream &out, const Body &body)
{
    out << "linVel: " << body.linearVelocity
        << ", pos: " << body.position << std::endl;
    for (auto shape : body.shapeList)
        out << *shape;
    return out;
}
} /* namespace phy */
