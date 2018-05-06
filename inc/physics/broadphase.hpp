#pragma once

#include "inc/physics/body.hpp"
#include "inc/physics/aabb.hpp"

#include <memory>
#include <utility>
#include <vector>
#include <unordered_set>

// Hash method for an integer pair with commutative properties
// pair_hash(<a, b>) == pair_hash(<b, a>)
struct pair_hash {
    inline std::size_t operator()(const std::pair<int32_t, int32_t> &p) const {
        return p.first ^ p.second;
    }
};

namespace phy {
class BroadPhase : public AABBCallback {
private:
    AABBTree tree;
    std::vector<std::pair<std::weak_ptr<const Shape>, int32_t>> shapeMapping;
    std::vector<std::pair<std::weak_ptr<const Body>, int32_t>> bodyMapping;
    std::unordered_set<std::pair<int32_t, int32_t>, pair_hash> collisions;
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
    std::vector<std::pair<std::weak_ptr<const Body>, std::weak_ptr<const Body>>>
    getBodyCollisions();
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
