#include "inc/physics/broadphase.hpp"

namespace phy {
int32_t BroadPhase::addAABB(const AABB &aabb)
{
    return tree.insertAABB(aabb);
}

void BroadPhase::updatePairs()
{

}

void BroadPhase::forceUpdate(int32_t id)
{

}

bool BroadPhase::registerCollision(const AABB &a, int32_t nodeID)
{
    return false;
}
} /* namespace phy */
