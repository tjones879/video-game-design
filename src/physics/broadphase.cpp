#include "inc/physics/broadphase.hpp"
#include <iostream>
#include <algorithm>

namespace phy {
BroadPhase::BroadPhase() : tree(10) {}

auto BroadPhase::findShape(const std::weak_ptr<const Shape> &shape)
{
    auto pos = std::find_if(std::begin(mapping), std::end(mapping),
            [shape](auto item) {
                return !item.first.owner_before(shape) && !shape.owner_before(item.first);
            });
    if (pos != std::end(mapping))
        return std::make_pair(true, pos);
    else
        return std::make_pair(false, pos);
}

void BroadPhase::addNewBody(const std::shared_ptr<Body> body)
{
    const auto transform = body->getTransform();
    for (auto shape : body->getShapes()) {
        const auto aabb = shape.lock()->getAABB(transform);
        mapping.emplace_back(shape, tree.insertAABB(aabb));
    }
}

void BroadPhase::updateBody(const std::shared_ptr<Body> updatedBody)
{
    const auto transform = updatedBody->getTransform();
    for (auto shape : updatedBody->getShapes()) {
        const auto aabb = shape.lock()->getAABB(transform);
        auto pos = findShape(shape);
        // If the shape is already known, update it.
        if (pos.first) {
            tree.updateAABB(pos.second->second, aabb);
            moved.push_back(pos.second->second);
        // Insert the shape's AABB otherwise.
        } else {
            mapping.emplace_back(shape, tree.insertAABB(aabb));
        }
    }
}

void BroadPhase::deleteBody(const std::shared_ptr<Body> deletedBody)
{
    for (auto shape : deletedBody->getShapes()) {
        auto find = findShape(shape);
        if (find.first)
            mapping.erase(find.second);
    }
}

void BroadPhase::updatePairs()
{
    for (auto index : moved) {
        tree.findCollisions(this, index);
    }

    moved.clear();
}

bool BroadPhase::registerCollision(const AABB &a, int32_t nodeID)
{
    return true;
}

void BroadPhase::printTree(std::ostream &out)
{
    out << tree;
}
} /* namespace phy */
