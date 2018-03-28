#pragma once

#include <vector>
#include "inc/physics/circle.hpp"
#include "inc/physics/polygon.hpp"
#include "inc/physics/common.hpp"

namespace phy {
/**
 * A manifold describes the properties of a collision between two objects.
 */
struct Manifold {
    enum class Type : char {
        circles,
        polygons,
        INVALID
    };

    Manifold()
        : type(Type::INVALID), depth(1000000) {}
    Vec2f localNormal;
    float depth;
    Type type;
};

Manifold collideCircles(const CircleShape &a, const Transform &transformA,
                        const CircleShape &b, const Transform &transformB);

Manifold collidePolygons(const PolygonShape &a, const Transform &transformA,
                         const PolygonShape &b, const Transform &transformB);
} /* namespace phy */
