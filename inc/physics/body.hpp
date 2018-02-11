#include <inc/physics/common.hpp>

namespace phy {

class Shape;
class ShapeSpec;
class Joint;
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
    Shape createShape(const ShapeSpec &spec);
    void DestroyShape(Shape *shape);
    const Vec2 getPosition() const;
    float getRoation() const;

    void setLinearVelocity(const Vec2 &velocity);
    const Vec2 &getLinearVelocity();

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

    Shape *getShapes();
    const Shape *getShapes() const;

    Joint *getJoints();
    const Joint *getJoints() const;

    Body *getNextBody();
    const Body *getNextBody() const;

    World *getParentWorld();
    const World *getParentWorld() const;
};
} /* namespace phy */
