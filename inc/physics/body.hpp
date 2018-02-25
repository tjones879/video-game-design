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
    void *extraData; ///< TODO: Change this to a structure specific to this game
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
    float gravityFactor; ///< This factor must be a positive nonzero value
    friend class World;
public:
    Body(const BodySpec &spec);
    ~Body();

    /**
     * Initialize the shape within this body using the specification.
     *
     * The specification can be safely destroyed to changed after
     * this function exits.
     */
    std::weak_ptr<Shape> createShape(const ShapeSpec &spec);

    /**
     * Remove the reference to this shape from the world.
     *
     * Note that this will not necessarily free the physical memory
     * until all other strong references have been cleared.
     */
    void destroyShape(const std::weak_ptr<Shape> &shape);
    const Vec2 &getPosition() const;

    /**
     * Get the angle of the body in radians.
     *
     * Note that this value is can be any real number.
     */
    float getRotation() const;

    void setLinearVelocity(const Vec2 &velocity);
    const Vec2 &getLinearVelocity() const;

    void setAngularVelocity(float velocity);
    float getAngularVelocity();

    void applyForce(const Vec2 &force_, const Vec2 &point);
    void applyTorque(float torque_);

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
    /**
     * Get any extra data about this body that other components may need to use.
     *
     * @return A void pointer that must be casted into the correct type.
     */
    void *getExtraData() const;
    /**
     * Set the pointer to this body's extra data.
     *
     * This data will never be used by the physics engine.
     */
    void setExtraData(void *data);
private:
    /**
     * Recalculate all mass and inertia characteristics of this body.
     *
     * This should be called whenever either
     *   (1) a new shape is added to the body
     *   (2) a shape's mass properties have changed
     */
    void updateMassProperties();
};
} /* namespace phy */
