#include <cstdlib>
#include <iostream>
#include "gtest/gtest.h"
#include "inc/physics/aabb.hpp"

using namespace phy;
using namespace std;

class AABBTest : public ::testing::Test {
protected:
    AABB a, b;
    virtual void SetUp()
    {
        srand(time(nullptr));
        a = AABB(Vec2f(rand(), rand()),
                 Vec2f(rand(), rand()));
        b = AABB(Vec2f(rand(), rand()),
                 Vec2f(rand(), rand()));
    }
};

TEST_F(AABBTest, ShouldCombineWithOneOther)
{
    auto result = this->a.combine(this->b);
    auto lower = minValues(this->a.lowVertex, this->b.lowVertex);
    auto higher = maxValues(this->a.highVertex, this->b.highVertex);
    auto expected = AABB(lower, higher);
    EXPECT_EQ(expected, result);
}

TEST_F(AABBTest, ShouldCombineWithManyOthers)
{
    auto third = AABB(Vec2f(rand(), rand()),
                      Vec2f(rand(), rand()));
    auto result = this->a.combine(this->b, third);

    auto lower = minValues(this->a.lowVertex, this->b.lowVertex);
    lower = minValues(lower, third.lowVertex);

    auto higher = maxValues(this->a.highVertex, this->b.highVertex);
    higher = maxValues(higher, third.highVertex);

    auto expected = AABB(lower, higher);
    EXPECT_EQ(expected, result);
}

TEST_F(AABBTest, ShouldSupportContains)
{
    auto result = this->a.contains(this->b);
    bool expected = true && this->a.lowVertex.x <= this->b.lowVertex.x;
    expected = expected && this->a.lowVertex.y <= this->b.lowVertex.y;
    expected = expected && this->a.highVertex.x >= this->b.highVertex.x;
    expected = expected && this->a.highVertex.y >= this->b.highVertex.y;
    EXPECT_EQ(expected, result);
}

TEST_F(AABBTest, ShouldSupportPerimeter)
{
    auto result = this->a.getPerimeter();
    auto x = this->a.highVertex.x - this->a.lowVertex.x;
    auto y = this->a.highVertex.y - this->a.lowVertex.y;
    EXPECT_EQ(2 * (x + y), result);
}

class AABBTreeTest : public ::testing::Test {
protected:
    AABBTree tree;
    virtual void SetUp()
    {
        srand(time(nullptr));
    }
};

TEST_F(AABBTreeTest, ShouldSupportBasicInsertion)
{
        AABB a(Vec2f(-5, -5),
               Vec2f(-1, -1));
        AABB b(Vec2f( 1,  1),
               Vec2f( 5,  5));
        this->tree.insertAABB(a);
        this->tree.insertAABB(b);
        // Root should contain both inserted AABB nodes and a root
        // node that contains the combination of both.
        auto expectedBox = a.combine(b);
        auto expectedHeight = 1;
        auto nodes = this->tree.getNodes();
        EXPECT_EQ(a, nodes[0].aabb);
        EXPECT_EQ(b, nodes[1].aabb);
        EXPECT_EQ(expectedBox, nodes[2].aabb);
        EXPECT_EQ(expectedHeight, nodes[2].height);
}

/*
TEST_F(AABBTreeTest, ShouldRemainBalanced)
{
    this->tree.insertAABB(AABB(Vec2f( 0,  0), Vec2f( 1, 1)));
    this->tree.insertAABB(AABB(Vec2f( 1,  2), Vec2f( 2, 3)));
    this->tree.insertAABB(AABB(Vec2f( 1, -1), Vec2f( 2, 0)));
    this->tree.insertAABB(AABB(Vec2f( 3,  1), Vec2f( 4, 2)));
    this->tree.insertAABB(AABB(Vec2f( 2,  3), Vec2f( 3, 4)));
    this->tree.insertAABB(AABB(Vec2f(-2, -1), Vec2f(-1, 0)));
    this->tree.insertAABB(AABB(Vec2f(-2,  1), Vec2f(-1, 2)));
    this->tree.insertAABB(AABB(Vec2f(0.5, -1), Vec2f(1.5, -0.5)));
    this->tree.insertAABB(AABB(Vec2f(0.5, -1), Vec2f(1.5, -0.5)));
}
*/

TEST_F(AABBTreeTest, ShouldSupportDestruction)
{
        AABB a(Vec2f(-5, -5),
               Vec2f(-1, -1));
        AABB b(Vec2f( 1,  1),
               Vec2f( 5,  5));
        auto indexA = this->tree.insertAABB(a);
        auto indexB = this->tree.insertAABB(b);
        this->tree.destroyAABB(indexA);
        auto nodes = this->tree.getNodes();
        // The node at indexA should be destroyed.
        EXPECT_EQ(AABBNode().height, nodes[0].height);
        // The new root should be equal to b. That is,
        // it should have no parent.
        EXPECT_EQ(-1, nodes[indexB].parent);
        // Node B should have no children. It should be
        // the only node in the tree.
        EXPECT_EQ(-1, nodes[indexB].leftChild);
        EXPECT_EQ(-1, nodes[indexB].rightChild);
        // The new root should have depth of 0.
        EXPECT_EQ(0, nodes[indexB].height);
        // The next index on the freelist for a should
        // remain the same: the original root before destroying A.
        EXPECT_EQ(2, nodes[indexA].next);
        EXPECT_EQ(2, nodes[indexB].next);
}

TEST_F(AABBTreeTest, ShouldSupportUpdates)
{
    this->tree.insertAABB(AABB(Vec2f( 0,  0), Vec2f( 1, 1)));
    this->tree.insertAABB(AABB(Vec2f( 1,  2), Vec2f( 2, 3)));
    this->tree.insertAABB(AABB(Vec2f( 1, -1), Vec2f( 2, 0)));
    auto index = this->tree.insertAABB(AABB(Vec2f( 3,  1), Vec2f( 4, 2)));
    this->tree.insertAABB(AABB(Vec2f( 2,  3), Vec2f( 3, 4)));
    this->tree.insertAABB(AABB(Vec2f(-2, -1), Vec2f(-1, 0)));

    auto newAABB = AABB(Vec2f(3, 0), Vec2f(4, 1));
    this->tree.updateAABB(index, newAABB);
    auto nodes = this->tree.getNodes();
    EXPECT_EQ(true, nodes[nodes[index].parent].aabb.contains(newAABB));
}
