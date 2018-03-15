#include "inc/physics/collisions.hpp"

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

} /*namespace phy */
