#pragma once

#include "inc/physics/shape.hpp"

namespace phy {

class EdgeShape : Shape {
public:
    EdgeShape();
    void Set(const Vec2f &vertex1, const Vec2f &vertex2);

    bool testPoint() const override;
    void getAABB() const override;
    MassProperties getMassProps() const override;

    Vec2f vertex1, vertex2;
};
} /* namespace phy */
