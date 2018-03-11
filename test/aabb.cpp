#include <cstdlib>
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

TEST_F(AABBTest, ShouldCombineWithOthers)
{
    auto result = this->a.combine(this->b);
    auto lower = minValues(this->a.lowVertex, this->b.lowVertex);
    auto higher = maxValues(this->a.highVertex, this->b.highVertex);
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
