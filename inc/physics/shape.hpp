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
    ShapeType shapeType;
public:
    virtual bool testPoint() const = 0;
    virtual void getAABB() const = 0;
    ShapeType getShapeType() const;
    virtual MassProperties getMassProps() const = 0;
};
} /* namespace phy */
