#pragma once

#include "inc/physics/common.hpp"

namespace phy {

enum class ShapeType {
    circle,
    edge,
    polygon,
};

struct MassProperties {
    float mass;
    Vec2 centroid;
    float inertia;
};

struct ShapeSpec {
    ShapeSpec();
    float friction;
    float density;
};

class Shape {
protected:
    ShapeType shapeType;
public:
    /**
     * Test a point to see if it is inside this shape.
     * @param point The location in world coordinates
     */
    virtual bool testPoint(const Transform &transform, const Vec2 &pos) const = 0;
    /**
     * Get the Axis aligned bounding box of this shape for
     * broad-phase collision detection.
     */
    virtual void getAABB() const = 0;
    ShapeType getShapeType() const;
    virtual MassProperties getMassProps() const = 0;
};
} /* namespace phy */
