#pragma once

#include "inc/physics/aabb.hpp"

namespace phy {
class BroadPhase : AABBCallback {
    AABBTree tree;
public:
    BroadPhase();
    int32_t addAABB(const AABB &aabb);
    void updatePairs();
    void forceUpdate(int32_t id);
    virtual bool registerCollision(const AABB &a, int32_t nodeID) override;
};
} /* namespace phy */
