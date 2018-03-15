#pragma once

#include "inc/physics/aabb.hpp"

namespace phy {
class BroadPhase {
    AABBTree tree;
public:
    BroadPhase();
    int32_t addAABB(const AABB &aabb);
    void updatePairs();
    void forceUpdate(int32_t id);
};
} /* namespace phy */
