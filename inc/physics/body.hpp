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
    BodySpec()
    {
        bodyType = BodyType::staticBody;
        angle = 0.0f;
        angVelocity = 0.0f;
        gravityFactor = 1.0f;
    }

    BodyType bodyType;
    Vec2 position;
    float angle;
    Vec2 linVelocity;
    float angVelocity;
    float gravityFactor;
};

class Body {
private:
    std::shared_ptr<World> parentWorld;
    float mass, invMass;
    float inertia, invInertia;
    BodyType bodyType;
    Vec2 position, centroid;
    Vec2 linearVelocity;
    float angle;
    float angularVelocity;
    Vec2 force;
    float torque;
    std::vector<std::shared_ptr<Shape>> shapeList;
    Sweep bodySweep;
    float gravityFactor; ///< Factor in the range [0, 1]
    friend class World;
public:
    Body(const BodySpec &spec);
    ~Body();

    std::weak_ptr<Shape> createShape(const ShapeSpec &spec);
    void destroyShape(std::weak_ptr<Shape> shape);
    const Vec2 &getPosition() const;

    /**
     * Get the angle of the body in radians.
     */
    float getRotation() const;

    void setLinearVelocity(const Vec2 &velocity);
    const Vec2 &getLinearVelocity() const;

    void setAngularVelocity(float velocity);
    float getAngularVelocity();

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

    /**
     * Update this body's velocity due to gravity and other forces.
     *
     * @param dt The amount of time in seconds since the last update
     * @param gravity A vector of the world's gravity effect
     */
    void updateVelocity(float dt, Vec2 gravity);
    /**
     * Update this body's position due to accumulated velocity.
     *
     * @param dt The amount of time in seconds since the last update
     */
    void updatePosition(float dt);
    /**
     * Remove all forces that are currently effecting the body.
     */
    void clearForces();
private:
    void updateMassProperties();
};
} /* namespace phy */
