#pragma once
#include "inc/physics/common.hpp"
#include "inc/physics/shape.hpp"
#include "inc/physics/circle.hpp"
#include "inc/physics/polygon.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

namespace phy {

class Shape;
class ShapeSpec;
class Contact;
class World;
class ExtraData;

enum class BodyType {
    staticBody,
    dynamicBody,
};

struct ExtraData {
    ExtraData() {
        color = {0, 125, 125, 255};
        colliding = false;
    }
    bool colliding;
    SDL_Color color;
    int colorAngle;
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
    Vec2f position; ///< World coordinates of this body
    float angle; ///< Rotation of this body in radians
    Vec2f linVelocity;
    float angVelocity;
    float gravityFactor; ///< Scalar factor for the world's gravity on this body
    std::vector<std::shared_ptr<Shape>> shapes;
    ExtraData extra;
};

class Body {
private:
    ExtraData extraData;
    std::shared_ptr<World> parentWorld;
    float mass, invMass;
    float inertia, invInertia;
    BodyType bodyType;
    Vec2f position, centroid;
    Vec2f linearVelocity;
    float angle;
    float angularVelocity;
    Vec2f force;
    float torque;
    std::vector<std::shared_ptr<Shape>> shapeList;
    Sweep bodySweep;
    float gravityFactor; ///< This factor must be a positive nonzero value
    Transform transform;
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
    std::weak_ptr<PolygonShape> addShape(const PolygonShape &shape);
    std::weak_ptr<CircleShape> addShape(const CircleShape &shape);

    /**
     * Remove the reference to this shape from the world.
     *
     * Note that this will not necessarily free the physical memory
     * until all other strong references have been cleared.
     */
    void destroyShape(const std::weak_ptr<Shape> &shape);
    const Vec2f &getPosition() const;

    /**
     * Get the angle of the body in radians.
     *
     * Note that this value is can be any real number.
     */
    float getRotation() const;

    void setLinearVelocity(const Vec2f &velocity);
    const Vec2f &getLinearVelocity() const;

    void setAngularVelocity(float velocity);
    float getAngularVelocity();

    /**
     * Apply a force in Newtons to a point in local space.
     *
     * @param force_ Force in newtons to apply in each direction
     * @param point  Point inside the body with local coordinates.
     */
    void applyForce(const Vec2f &force_, const Vec2f &point);

    /**
     * Apply some amount of torque to the centroid of the body.
     */
    void applyTorque(float torque_);

    void applyLinearImpulse(const Vec2f &impulse, const Vec2f &point);
    void applyAngularImpulse(float impulse);

    float getMass() const;
    float getInertia() const;
    const Vec2f &getCenterMass() const;

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
    void updateVelocity(float dt, Vec2f gravity);

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
     */
    ExtraData *getExtraData();

    /**
     * Set the pointer to this body's extra data.
     *
     * This data will never be used by the physics engine.
     */
    void setExtraData(ExtraData data);

    /**
     * Get the transformation needed to convert local shapes to global coords.
     */
    Transform getTransform() const;
private:
    /**
     * Recalculate all mass and inertia characteristics of this body.
     *
     * This should be called whenever either
     *   (1) a new shape is added to the body
     *   (2) a shape's mass properties have changed
     */
    void updateMassProperties();
    friend std::ostream& operator<<(std::ostream &out, const Body &body);
};

} /* namespace phy */
