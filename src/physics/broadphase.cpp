#include "inc/physics/broadphase.hpp"

namespace phy {
auto BroadPhase::findShape(std::weak_ptr<const Shape> shape)
{
    auto pos = std::find(std::begin(mapping), std::end(mapping),
            [shape](auto item) -> bool {
                item.first == shape;
            });
    if (pos != std::end(mapping))
        return std::make_pair(true, pos);
    else
        return std::make_pair(false, pos);
}
    /*
    std::vector<std::pair<const std::weak_ptr<const Shape>, int32_t>> mapping;
     */
void BroadPhase::addNewBody(const Body *body)
{
    const auto transform = body->getTransform();
    for (auto shape : body->getShapes()) {
        const auto aabb = shape.lock()->getAABB(transform);
        mapping.emplace_back(shape, tree.insertAABB(aabb));
    }
}

void BroadPhase::updateBody(const Body *updatedBody)
{
    const auto transform = updatedBody->getTransform();
    for (auto shape : updatedBody->getShapes()) {
        const auto aabb = shape.lock()->getAABB(transform);
        auto pos = findShape(shape);
        // If the shape is already known, update it.
        if (pos.first) {
            tree.updateAABB(pos.second->second, aabb);
        // Insert the shape's AABB otherwise.
        } else {
            mapping.emplace_back(shape, tree.insertAABB(aabb));
        }
    }
}

void BroadPhase::deleteBody(const Body *deletedBody)
{
    for (auto shape : deletedBody->getShapes()) {
        auto find = findShape(shape);
        if (find.first)
            mapping.erase(find.second);
    }
}

void BroadPhase::updatePairs()
{
    // For each recently updated pair:
    /* The world should always call BroadPhase::deleteBody() before deleting a body
     * but check to delete any expired shapes regardless. */

    //   Check for collisions
}

bool BroadPhase::registerCollision(const AABB &a, int32_t nodeID)
{
    return true;
}
} /* namespace phy */
