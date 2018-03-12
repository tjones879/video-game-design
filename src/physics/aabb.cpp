#include "inc/physics/aabb.hpp"

namespace phy {
Vec2f AABB::getCenter() const
{
    return 0.5f * (lowVertex + highVertex);
}

float AABB::getPerimeter() const
{
    float x = highVertex.x - lowVertex.x;
    float y = highVertex.y - lowVertex.y;
    return 2.f * (x + y);
}

AABB AABB::combine(const AABB &b)
{
    return {minValues(lowVertex, b.lowVertex),
            maxValues(highVertex, b.highVertex)};
}

bool AABB::contains(const AABB &other)
{
    return lowVertex.below(other.lowVertex) && highVertex.above(other.highVertex);
}

bool operator==(const AABB &a, const AABB& b)
{
    return (a.lowVertex == b.lowVertex)
        && (a.highVertex == b.highVertex);
}

std::ostream &operator<<(std::ostream &out, const AABB &aabb)
{
    out << "Lower: " << aabb.lowVertex
        << "Higher: " << aabb.highVertex << std::endl;
    return out;
}
} /* namespace phy */
