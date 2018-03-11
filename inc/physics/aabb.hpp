#pragma once

#include "inc/physics/common.hpp"
#include <ostream>

namespace phy {
class AABB {
public:
    Vec2f lowVertex, highVertex;

    AABB() {}
    AABB(Vec2f lower, Vec2f higher)
        : lowVertex(lower), highVertex(higher) {}

    Vec2f getCenter() const
    {
        return 0.5f * (lowVertex + highVertex);
    }

    float getPerimeter() const
    {
        float x = highVertex.x - lowVertex.x;
        float y = highVertex.y - lowVertex.y;
        return 2.f * (x + y);
    }

    AABB combine(const AABB &b)
    {
        return AABB(minValues(lowVertex, b.lowVertex),
                    maxValues(highVertex, b.highVertex));
    }

    template <class T, class... Args>
    AABB combine(T first, Args&&... args)
    {
        return combine(args...);
    }

    /**
     * Determine if this AABB contains the given AABB.
     */
    bool contains(const AABB &other)
    {
        return true;
    }
};

bool operator==(const AABB &a, const AABB& b)
{
    return (a.lowVertex == b.lowVertex) && (a.highVertex == b.highVertex);
}

std::ostream &operator<<(std::ostream &out, const AABB &aabb)
{
    out << "Lower: " << aabb.lowVertex
        << "Higher: " << aabb.highVertex << std::endl;
    return out;
}
} /* namespace phy */
