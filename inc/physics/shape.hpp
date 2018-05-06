#pragma once

#include "inc/physics/aabb.hpp"
#include "inc/physics/common.hpp"
#include <ostream>
#include <memory>

namespace phy {

enum class ShapeType {
    circle,
    polygon,
    INVALID
};

struct MassProperties {
    float mass;
    Vec2f centroid;
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
    Shape() : shapeType(ShapeType::INVALID) {}
    Shape(ShapeType type) : shapeType(type) {}
    /**
     * Test a point to see if it is inside this shape.
     * @param point The location in world coordinates
     */
    virtual bool testPoint(const Transform &transform, const Vec2f &pos) const = 0;
    /**
     * Get the Axis aligned bounding box of this shape for broad-phase
     * collision detection.
     */
    virtual AABB getAABB(const Transform &transform) const = 0;
    ShapeType getShapeType() const;
    virtual MassProperties getMassProps() const = 0;
    virtual void print(std::ostream &out) const = 0;
};

std::ostream &operator<<(std::ostream &out, const Shape &shape);
} /* namespace phy */
