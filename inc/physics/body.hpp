#pragma once
#include "inc/physics/common.hpp"
#include "inc/physics/shape.hpp"
#include <memory>
#include <vector>

namespace phy {

class Shape;
class ShapeSpec;
class Contact;
class World;

enum class BodyType {
    staticBody,
    dynamicBody,
};

struct BodySpec {
    BodySpec();

    BodyType bodyType;
    Vec2 position;
    float angle;
    Vec2 linVelocity;
    Vec2 angVelocity;
    float gravityFactor;
};

class Body {
private:
    std::shared_ptr<World> parentWorld;
    Vec2 position, linearVelocity, angularVelocity, centroid;
    std::vector<std::shared_ptr<Shape>> shapeList;
public:
    Body(const BodySpec &spec);
    ~Body();

    std::weak_ptr<Shape> createShape(const ShapeSpec &spec);
    void destroyShape(std::weak_ptr<Shape> shape);
    const Vec2 &getPosition() const;
    float getRotation() const;

    void setLinearVelocity(const Vec2 &velocity);
    const Vec2 &getLinearVelocity() const;

    void setAngularVelocity(const Vec2 &velocity);
    const Vec2 &getAngularVelocity();

    void applyForce(const Vec2 &force, const Vec2 &point);
    void applyTorque(float torque);

    void applyLinearImpulse(const Vec2 &impulse, const Vec2 &point);
    void applyAngularImpulse(float impulse);

    float getMass() const;
    float getInertia() const;
    const Vec2 &getCenterMass() const;

    void setSleep();
    bool isAsleep();

    std::vector<std::weak_ptr<Shape>> getShapes();
    std::vector<std::weak_ptr<const Shape>> getShapes() const;

    std::weak_ptr<World> getParentWorld();
    std::weak_ptr<const World> getParentWorld() const;
};
} /* namespace phy */
