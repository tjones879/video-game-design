#pragma once

#include "inc/physics/common.hpp"
#include <ostream>
#include <vector>
#include <string>

namespace phy {
class AABB {
public:
    Vec2f lowVertex, highVertex;

    AABB() {}
    AABB(Vec2f lower, Vec2f higher)
        : lowVertex(lower), highVertex(higher) {}

    Vec2f getCenter() const;
    float getPerimeter() const;
    float getArea() const;
    /**
     * Get the size of the AABB in each direction.
     */
    Vec2f getSideLengths() const;
    AABB combine(const AABB &b) const;

    template <class T, class... Args>
    AABB combine(T first, Args&&... args) const
    {
        return combine(first).combine(args...);
    }

    /**
     * Determine if this AABB contains the given AABB.
     *
     * @return True if the other AABB is completely inside.
     */
    bool contains(const AABB &other) const;
    /**
     * Determine if this AABB overlaps with the given AABB.
     *
     * @return True if the other AABB overlaps with this.
     */
    bool overlaps(const AABB &other) const;
};

bool operator==(const AABB &a, const AABB& b);
std::ostream &operator<<(std::ostream &out, const AABB &aabb);

/**
 * An AABBNode is either:
 *  1) A branch with two valid children. The AABB for this node
 *     must be a fat AABB containing both children.
 *  2) A leaf with the real AABB of a shape in the world.
 */
class AABBNode {
public:
    AABB aabb;
    static const int32_t null;
    int32_t parent; // Index of the parent node
    int32_t leftChild; // Index of left child (if not a leaf)
    int32_t rightChild; // Index of right child (if not a leaf)

    int32_t next; // Index of the next node in the freelist
    int32_t height; // Height of the subtree at this origin

    AABBNode()
        : parent(null), leftChild(null), rightChild(null),
          height(-1) {}
    bool isLeaf() const
    {
        return rightChild == null;
    }
};

std::ostream &operator<<(std::ostream &out, const AABBNode &b);

class AABBCallback {
public:
    /**
     * Callback for when the dynamic tree finds an overlap
     * in AABB.
     */
    virtual bool registerCollision(int32_t nodeA, int32_t nodeB) = 0;
};


/**
 * A binary search tree containing all AABB in the world.
 *
 * The binary tree uses an internal std::vector to store
 * all AABB in contiguous storage and uses indices to
 * reference them.
 */
class AABBTree {
    std::vector<AABBNode> nodes;
    int32_t root; // Index of root node
    int32_t nextFreeIndex;
public:
    AABBTree();
    AABBTree(size_t initialSize);

    int32_t insertAABB(const AABB &box);
    void updateAABB(int32_t index, const AABB &newAABB);
    void destroyAABB(int32_t index);
    /**
     * Find any any AABB in the tree that overlap with the
     * one that is given.
     * @note This prototype should only be used for testing and will
     *       always return -1 for the first index on callback.
     */
    void findCollisions(AABBCallback *callback, const AABB &aabb) const;
    /**
     * Find any any AABB in the tree that overlap with the
     * one that is given.
     * @note This is the only prototype that should be used for traversing
     *       the tree of ingame objects and will use the correct index on
     *       callback.
     */
    void findCollisions(AABBCallback *callback, int32_t index) const;
    /**
     * Convert the tree to a string for debugging purposes.
     */
    std::string dump() const;
    /**
     * Get the list of nodes for testing purposes.
     */
    std::vector<AABBNode> getNodes() const;
    const AABB operator[](int i) const;
private:
    /**
     * Attempt to remove a node from the tree.
     *
     * This is operation is only valid when performed on
     * leaf nodes. That is, any node that is not a fattened
     * AABB.
     */
    void remove(int32_t node);
    void freeNode(int32_t node);
    /**
     * Balance the subtree at the given origin.
     *
     * @return The index of the new root.
     */
    int32_t balance(int32_t origin);
    /**
     * Find a position for a new AABB inside the internal vector.
     */
    int32_t allocateNode();
    /**
     * Find the correct position in the tree for an allocated node.
     */
    void insertNode(int32_t node);
};

std::ostream &operator<<(std::ostream &out, AABBTree tree);
} /* namespace phy */
