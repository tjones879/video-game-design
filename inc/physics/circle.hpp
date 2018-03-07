#pragma once

#include "inc/physics/shape.hpp"
#include <math.h>

namespace phy {
class CircleShape : public Shape {
    float density;
    float radius;
    Vec2f pos;
public:
    CircleShape();
    CircleShape(const Vec2f& pos, float radius);

    virtual bool testPoint(const Transform &transform, const Vec2f &pos) const override;
    virtual void getAABB() const override;
    virtual MassProperties getMassProps() const override;
};
} /* namespace phy */
