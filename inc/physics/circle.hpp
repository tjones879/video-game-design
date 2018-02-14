#pragma once

#include "inc/physics/shape.hpp"

namespace phy {
class CircleShape : public Shape {
    float radius;
    Vec2 pos;
public:
    CircleShape();
    CircleShape(const Vec2& pos, float radius);

    bool testPoint() const override;
    void getAABB() const override;
    MassProperties getMassProps() const override;
};
} /* namespace phy */
