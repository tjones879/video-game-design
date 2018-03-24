#include "inc/physics/collisions.hpp"
#include <iostream>

namespace phy {
Manifold collideCircles(const CircleShape &a, const Transform &transformA,
                        const CircleShape &b, const Transform &transformB)
{
    Manifold manifold;

    Vec2f posA = transformA.translate(a.pos);
    Vec2f posB = transformB.translate(b.pos);
    Vec2f distance = posB - posA;
    float distSquared = distance.length();
    float radius = a.radius + b.radius;
    if (distSquared > radius * radius)
        return manifold;

    manifold.type = Manifold::Type::circles;
    // TODO: Find MTV
    return manifold;
}

Manifold collidePolygons(const PolygonShape &a, const Transform &transformA,
                         const PolygonShape &b, const Transform &transformB)
{
    Manifold manifold;
    std::pair<Vec2f, float> pen;
    auto polyA = PolygonShape(a, transformA);
    auto axesA = polyA.getNormals();
    auto polyB = PolygonShape(b, transformB);
    auto axesB = polyB.getNormals();

    auto overlaps = [](const auto &p1, const auto &p2) -> bool {
        return !(p1.first > p2.second || p2.first > p1.second);
    };
    auto getOverlap = [](const auto &p1, const auto &p2) {
        return p1.first - p2.second;
    };

    for (auto axis : axesA) {
        auto p1 = polyA.projectShape(axis);
        auto p2 = polyB.projectShape(axis);

        if (!overlaps(p1, p2)) {
            return manifold;
        } else {
            auto depth = getOverlap(p1, p2);
            if (depth > pen.second) {
                pen.first = axis;
                pen.second = depth;
            }
        }
    }

    for (auto axis : axesB) {
        auto p1 = polyA.projectShape(axis);
        auto p2 = polyB.projectShape(axis);

        if (!overlaps(p1, p2)) {
            return manifold;
        } else {
            auto depth = getOverlap(p1, p2);
            if (depth > pen.second) {
                pen.first = axis;
                pen.second = depth;
            }
        }
    }

    manifold.type = Manifold::Type::polygons;
    manifold.penetration = pen;
    return manifold;
}
} /*namespace phy */
