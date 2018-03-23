#pragma once

#include <vector>
#include "inc/physics/circle.hpp"
#include "inc/physics/polygon.hpp"
#include "inc/physics/common.hpp"

namespace phy {

struct ManifoldPoint {
    Vec2f localPoint;
    float normalImpulse;
    float frictionImpulse;
};

/**
 * A manifold describes the properties of a collision between two objects.
 */
struct Manifold {
    enum class Type : char {
        circles,
        polygons,
        INVALID
    };
    static const size_t maxPoints = 2;

    Manifold()
        : type(Type::INVALID), points(maxPoints) {}
    std::vector<ManifoldPoint> points;
    Vec2f localNormal;
    Vec2f localPoint;
    Type type;
};

Manifold collideCircles(const CircleShape &a, const Transform &transformA,
                        const CircleShape &b, const Transform &transformB);

Manifold collidePolygons(const PolygonShape &a, const Transform &transformA,
                         const PolygonShape &b, const Transform &transformB);
} /* namespace phy */
