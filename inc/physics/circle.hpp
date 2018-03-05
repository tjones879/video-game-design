#pragma once

#include "inc/physics/shape.hpp"
#include <math.h>

namespace phy {
class CircleShape : public Shape {
    float density;
    float radius;
    Vec2 pos;
public:
    CircleShape();
    CircleShape(const Vec2& pos, float radius);

    virtual bool testPoint(const Transform &transform, const Vec2 &pos) const override;
    virtual void getAABB() const override;
    virtual MassProperties getMassProps() const override;
    virtual void print(std::ostream &out) const override;
};
} /* namespace phy */
