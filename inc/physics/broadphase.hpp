#pragma once

#include "inc/physics/body.hpp"
#include "inc/physics/aabb.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace phy {
class BroadPhase : public AABBCallback {
private:
    AABBTree tree;
    std::vector<std::pair<std::weak_ptr<const Shape>, int32_t>> shapeMapping;
    std::vector<std::pair<std::weak_ptr<const Body>, int32_t>> bodyMapping;
    std::vector<int32_t> moved;
public:
    BroadPhase();
    /**
     * Insert a new body and all of its shapes to the broadphase manager.
     */
    void addNewBody(const std::shared_ptr<Body> body);
    /**
     * Update the position of a shape.
     */
    void updateBody(const std::shared_ptr<Body> updatedBody);
    /**
     * Remove a body from any future broadphase calculations.
     */
    void deleteBody(const std::shared_ptr<Body> deletedBody);
    void updatePairs();
    virtual bool registerCollision(int32_t nodeA, int32_t nodeB) override;
    void printTree(std::ostream &out);
private:
    /**
     * Attempt to find the position of a shape being managed.
     *
     * @return A pair:
     *      the first value denotes whether the shape was found.
     *      the second value is the result of std::find().
     */
    auto findShape(const std::weak_ptr<const Shape> &shape);
};
} /* namespace phy */
