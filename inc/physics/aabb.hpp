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

    Vec2f getCenter() const;
    float getPerimeter() const;
    AABB combine(const AABB &b);

    template <class T, class... Args>
    AABB combine(T first, Args&&... args)
    {
        return combine(first).combine(args...);
    }

    /**
     * Determine if this AABB contains the given AABB.
     *
     * @return True if the other AABB is completely inside.
     */
    bool contains(const AABB &other);
};

bool operator==(const AABB &a, const AABB& b);
std::ostream &operator<<(std::ostream &out, const AABB &aabb);
} /* namespace phy */
