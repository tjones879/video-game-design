#include "inc/physics/aabb.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <strstream>

namespace phy {
// Declare the null constant outside the struct
// to avoid linker errors.
int32_t const AABBNode::null = -1;

Vec2f AABB::getCenter() const
{
    return 0.5f * (lowVertex + highVertex);
}

float AABB::getPerimeter() const
{
    float x = highVertex.x - lowVertex.x;
    float y = highVertex.y - lowVertex.y;
    return 2.f * (x + y);
}

AABB AABB::combine(const AABB &b) const
{
    return {minValues(lowVertex, b.lowVertex),
            maxValues(highVertex, b.highVertex)};
}

bool AABB::contains(const AABB &other) const
{
    return lowVertex.below(other.lowVertex) && highVertex.above(other.highVertex);
}

bool operator==(const AABB &a, const AABB& b)
{
    return (a.lowVertex == b.lowVertex)
        && (a.highVertex == b.highVertex);
}

std::ostream &operator<<(std::ostream &out, const AABB &aabb)
{
    out << "Lower: " << aabb.lowVertex
        << "Higher: " << aabb.highVertex << std::endl;
    return out;
}

std::ostream &operator<<(std::ostream &out, const AABBNode &b)
{
    out << "Parent: " << std::setw(3) << b.parent
        << std::setw(11) << " leftChild: " << std::setw(3) << b.leftChild
        << std::setw(11) << " rightChild: " << std::setw(3) << b.rightChild
        << std::setw(7) << " next: " << std::setw(3) << b.next
        << std::setw(9) << " height: " << std::setw(3) << b.height
        << "aabb: " << b.aabb;
    return out;
}

AABBTree::AABBTree()
    : AABBTree(10) {}

AABBTree::AABBTree(size_t initialSize)
    : nodes(initialSize), nextFreeIndex(0)
{
    root = AABBNode::null;
    for (size_t i = 0; i < nodes.capacity(); i++) {
        nodes[i].next = i + 1;
        nodes[i].height = -1;
    }

    nodes[nodes.capacity() - 1].next = AABBNode::null;
}

int32_t AABBTree::insertAABB(const AABB &box)
{
    int32_t index = allocateNode();
    // TODO: Fatten the aabb.
    nodes[index].aabb = box;
    nodes[index].height = 0;

    insertNode(index);
    return index;
}

void AABBTree::destroyAABB(int32_t index)
{
    if (index < 0 || index > nodes.capacity())
        return;

    remove(index);
    // Reinitialize this index,it is a performance loss,
    // but makes testing/bug finding easier.
    nodes[index].height = -1;
    freeNode(index);
}

void AABBTree::updateAABB(int32_t index, const AABB &newAABB)
{
    if (index < 0 || index > nodes.capacity())
        return;

    if (nodes[index].aabb.contains(newAABB)) {
        return;
    }

    // Just remove, update, reinsert the node.
    // TODO: Find a more efficient way of updating nodes.
    remove(index);
    nodes[index].aabb = newAABB;
    insertNode(index);
}

int32_t AABBTree::allocateNode()
{
    // Expand the node pool if necessary
    if (nextFreeIndex == AABBNode::null ||
        nextFreeIndex >= nodes.capacity()) {
        auto oldSize = nodes.capacity();
        nodes.resize(oldSize * 2);

        for (int32_t i = oldSize - 1; i < nodes.capacity(); i++) {
            nodes[i].next = i + 1;
            nodes[i].height = -1;
        }

        nodes[nodes.capacity() - 1].next = AABBNode::null;
        nextFreeIndex = oldSize;
    }

    int32_t index = nextFreeIndex;
    nextFreeIndex = nodes[index].next;
    return index;
}

void AABBTree::insertNode(int32_t node)
{
    if (root == AABBNode::null) {
        root = node;
        nodes[root].parent = AABBNode::null;
        return;
    }

    auto newAABB = nodes[node].aabb;
    int32_t index = root;
    // Attempt to find the best sibling node.
    while (!nodes[index].isLeaf()) {
        int32_t left = nodes[index].leftChild;
        int32_t right = nodes[index].rightChild;

        float area = nodes[index].aabb.getPerimeter();
        auto combined = nodes[index].aabb.combine(newAABB);
        float combinedArea = combined.getPerimeter();

        float cost = 2 * combinedArea;
        float inheritanceCost = 2 * (combinedArea - area);

        // Cost of descending into the given child
        auto childCost = [this, &inheritanceCost, &newAABB] (int32_t child) -> float {
            auto childAABB = newAABB.combine(nodes[child].aabb);

            float cost = childAABB.getPerimeter() + inheritanceCost;
            if (!nodes[child].isLeaf()) {
                float oldArea = nodes[child].aabb.getPerimeter();
                cost -= oldArea;
            }
            return cost;
        };

        float costLeft = childCost(left);
        float costRight = childCost(right);
        if (cost < costLeft && cost < costRight)
            break;

        index = costLeft < costRight ? left : right;
    }

    int32_t sibling = index;

    // Create a new parent node for these siblings.
    int32_t oldParent = nodes[sibling].parent;
    int32_t newParent = allocateNode();
    nodes[newParent].parent = oldParent;
    nodes[newParent].aabb = newAABB.combine(nodes[sibling].aabb);
    nodes[newParent].height = nodes[sibling].height + 1;

    // Check if the sibling node was root, update accordingly.
    if (oldParent != AABBNode::null) {
        if (nodes[oldParent].leftChild == sibling)
            nodes[oldParent].leftChild = newParent;
        else
            nodes[oldParent].rightChild = newParent;
    } else {
        root = newParent;
    }
    nodes[newParent].leftChild = sibling;
    nodes[newParent].rightChild = node;
    nodes[sibling].parent = newParent;
    nodes[node].parent = newParent;

    index = nodes[node].parent;
    while (index != AABBNode::null) {
        index = balance(index);

        int32_t leftChild = nodes[index].leftChild;
        int32_t rightChild = nodes[index].rightChild;

        nodes[index].height = 1 + std::max(nodes[leftChild].height, nodes[rightChild].height);
        nodes[index].aabb = nodes[index].aabb.combine(nodes[leftChild].aabb, nodes[rightChild].aabb);

        index = nodes[index].parent;
    }
}

void AABBTree::remove(int32_t index)
{
    if (index > nodes.capacity() ||
        !nodes[index].isLeaf())
        return;

    if (index == root) {
        root = AABBNode::null;
        return;
    }

    int32_t parent = nodes[index].parent;
    int32_t grandParent = nodes[parent].parent;
    int32_t sibling;
    if (nodes[parent].leftChild == index)
        sibling = nodes[parent].rightChild;
    else
        sibling = nodes[grandParent].leftChild;

    if (grandParent != AABBNode::null) {
        if (nodes[grandParent].leftChild == parent)
            nodes[grandParent].leftChild = sibling;
        else
            nodes[grandParent].rightChild = sibling;

        nodes[sibling].parent = grandParent;
        freeNode(parent);

        int32_t index = grandParent;
        while (index != AABBNode::null) {
            index = balance(index);

            int32_t left = nodes[index].leftChild;
            int32_t right = nodes[index].rightChild;

            nodes[index].aabb = nodes[index].aabb.combine(nodes[left].aabb, nodes[right].aabb);
            nodes[index].height = 1 + std::max(nodes[left].height, nodes[right].height);

            index = nodes[index].parent;
        }
    } else {
        root = sibling;
        nodes[sibling].parent = AABBNode::null;
        freeNode(parent);
    }
}

int32_t AABBTree::balance(int32_t index)
{
    struct NodePair {
        int32_t index;
        AABBNode *ptr;
        NodePair(int32_t index_, std::vector<AABBNode> &nodes)
            : index(index_), ptr(&nodes[index]) {}
        operator int32_t() {
            return index;
        }
        AABBNode *operator()() {
            return ptr;
        }
    };

    if (index == AABBNode::null)
        return AABBNode::null;

    auto input = NodePair(index, nodes);
    if (input()->isLeaf() || input()->height < 2)
        return index;

    auto left = NodePair(input()->leftChild, nodes);
    auto right = NodePair(input()->rightChild, nodes);
    int32_t balance = right()->height - left()->height;
    std::cout << "Balance is: " << balance << std::endl;
    std::cout << "Input:      " << *input();
    std::cout << "Left:       " << *left();
    std::cout << "Right:      " << *input();

    // The right child is longer than the left, so rotate it
    if (balance > 1) {
        auto grandLeft = NodePair(right()->leftChild, nodes);
        auto grandRight = NodePair(right()->rightChild, nodes);
        std::cout << "grandLeft:  " << *input();
        std::cout << "grandRight: " << *input();

        right()->leftChild = input;
        right()->parent = input()->parent;
        input()->parent = right;

        if (right()->parent != AABBNode::null) {
            // input's old parent should point to right
            if (nodes[right()->parent].leftChild == input)
                nodes[right()->parent].leftChild = right;
            else
                nodes[right()->parent].rightChild = right;
        } else {
            root = right;
        }

        if (grandLeft()->height > grandRight()->height) {
            right()->rightChild = grandLeft;
            input()->rightChild = grandRight;
            grandRight()->parent = input;
            input()->aabb = input()->aabb.combine(left()->aabb, grandRight()->aabb);
            right()->aabb = right()->aabb.combine(input()->aabb, grandLeft()->aabb);

            input()->height = 1 + std::max(left()->height, grandRight()->height);
            right()->height = 1 + std::max(input()->height, grandLeft()->height);
        } else {
            right()->rightChild = grandRight;
            input()->rightChild = grandLeft;
            grandLeft()->parent = input;
            input()->aabb = input()->aabb.combine(left()->aabb, grandLeft()->aabb);
            right()->aabb = right()->aabb.combine(input()->aabb, grandRight()->aabb);

            input()->height = 1 + std::max(left()->height, grandLeft()->height);
            right()->height = 1 + std::max(input()->height, grandRight()->height);
        }

        return right;
    }
    // The subtree at the left child is smaller than right, rotate it up
    if (balance < -1) {
        auto grandLeft = NodePair(left()->leftChild, nodes);
        auto grandRight = NodePair(left()->rightChild, nodes);
        std::cout << "grandLeft:  " << *input();
        std::cout << "grandRight: " << *input();

        left()->leftChild = input;
        left()->parent = input()->parent;
        input()->parent = left;

        if (left()->parent != AABBNode::null) {
            if (nodes[left()->parent].leftChild == input)
                nodes[left()->parent].leftChild = left;
            else
                nodes[left()->parent].rightChild = left;
        } else {
            root = left;
        }

        if (grandLeft()->height > grandRight()->height) {
            left()->rightChild = grandLeft;
            input()->leftChild = grandRight;
            grandRight()->parent = input;

            input()->aabb = input()->aabb.combine(right()->aabb, grandRight()->aabb);
            left()->aabb = left()->aabb.combine(input()->aabb, grandLeft()->aabb);

            input()->height = 1 + std::max(right()->height, grandRight()->height);
            left()->height = 1 + std::max(input()->height, grandLeft()->height);
        } else {
            left()->rightChild = grandRight;
            input()->leftChild = grandLeft;
            grandLeft()->parent = input;

            input()->aabb = input()->aabb.combine(right()->aabb, grandLeft()->aabb);
            left()->aabb = left()->aabb.combine(input()->aabb, grandRight()->aabb);

            input()->height = 1 + std::max(right()->height, grandLeft()->height);
            left()->height = 1 + std::max(input()->height, grandRight()->height);
        }

        return left;
    }

    return input;
}

void AABBTree::freeNode(int32_t node)
{
    if (node == AABBNode::null || node > nodes.capacity())
        return;

    nodes[node].next = nextFreeIndex;
    nodes[node].height = -1;
    nextFreeIndex = node;
}

std::vector<AABBNode> AABBTree::getNodes() const
{
    return nodes;
}

std::string AABBTree::dump() const
{
    if (root == AABBNode::null)
        return "This tree is empty";

    std::strstream sstream;

    sstream << "root: " << root
            << "nextFreeIndex: " << nextFreeIndex
            << std::endl;

    for (const auto box : nodes)
        sstream << box;
    return sstream.str();
}

std::ostream &operator<<(std::ostream &out, AABBTree tree)
{
    out << tree.dump() << std::endl;
    return out;
}
} /* namespace phy */
