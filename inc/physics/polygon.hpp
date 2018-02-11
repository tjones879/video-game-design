#pragma once

#include "inc/physics/shape.hpp"

const unsigned maxPolygonVertices = 255;

namespace phy {
class PolygonShape : public Shape {
public:
    PolygonShape();
    bool testPoint() const override;
    void getAABB() const override;
    MassProperties getMassProps() const override;

    Vec2 centroid;
    Vec2 vertices[maxPolygonVertices];
    Vec2 normals[maxPolygonVertices];
};
} /* namespace phy */
