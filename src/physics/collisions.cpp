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
    manifold.localPoint = a.pos;
    manifold.points[0].localPoint = b.pos;
    return manifold;
}

Manifold collidePolygons(const PolygonShape &a, const Transform &transformA,
                         const PolygonShape &b, const Transform &transformB)
{
    Manifold manifold;

    auto polyA = PolygonShape(a, transformA);
    auto axesA = polyA.getNormals();
    auto polyB = PolygonShape(b, transformB);
    auto axesB = polyB.getNormals();

    auto overlaps = [](const auto &p1, const auto &p2) -> bool {
        return !(p1.first > p2.second || p2.first > p1.second);
    };

    for (auto axis : axesA) {
        auto p1 = polyA.projectShape(axis);
        auto p2 = polyB.projectShape(axis);

        if (!overlaps(p1, p2)) {
            return manifold;
        }
    }

    for (auto axis : axesB) {
        auto p1 = polyA.projectShape(axis);
        auto p2 = polyB.projectShape(axis);

        if (!overlaps(p1, p2)) {
            return manifold;
        }
    }

    manifold.type = Manifold::Type::polygons;
    return manifold;
}
} /*namespace phy */
