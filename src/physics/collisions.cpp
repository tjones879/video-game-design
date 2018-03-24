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
    auto polyA = PolygonShape(a, transformA);
    auto axesA = polyA.getNormals();
    auto polyB = PolygonShape(b, transformB);
    auto axesB = polyB.getNormals();

    auto overlaps = [](const auto &p1, const auto &p2) -> bool {
        return !(p1.first > p2.second || p2.first > p1.second);
    };
    auto getOverlap = [](const auto &p1, const auto &p2) {
        return std::min(p1.second - p2.first, p2.second - p1.first);
    };

    for (auto axis : axesA) {
        auto normalized = axis.normalize();
        auto p1 = polyA.projectShape(normalized);
        auto p2 = polyB.projectShape(normalized);

        if (!overlaps(p1, p2)) {
            return manifold;
        } else {
            auto depth = getOverlap(p1, p2);
            std::cout << "Axis: " << normalized << "Pen depth: " << depth << std::endl;
            if (depth < manifold.depth) {
                manifold.localNormal = normalized;
                manifold.depth = depth;
            }
        }
    }

    for (auto axis : axesB) {
        auto normalized = axis.normalize();
        auto p1 = polyA.projectShape(normalized);
        auto p2 = polyB.projectShape(normalized);

        if (!overlaps(p1, p2)) {
            return manifold;
        } else {
            auto depth = getOverlap(p1, p2);
            std::cout << "Axis: " << normalized << "Pen depth: " << depth << std::endl;
            if (depth < manifold.depth) {
                manifold.localNormal = normalized;
                manifold.depth = depth;
            }
        }
    }

    manifold.type = Manifold::Type::polygons;
    return manifold;
}
} /*namespace phy */
