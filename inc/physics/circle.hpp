#pragma once

#include "inc/physics/shape.hpp"
#include <math.h>

namespace phy {
class CircleShape : public Shape {
public:
    float density;
    float radius;
    Vec2f pos;

    CircleShape();
    CircleShape(const Vec2f& pos, float radius);

    virtual bool testPoint(const Transform &transform, const Vec2f &pos) const override;
    virtual AABB getAABB(const Transform &transform) const override;
    virtual MassProperties getMassProps() const override;
    virtual void print(std::ostream &out) const override;
};
} /* namespace phy */
