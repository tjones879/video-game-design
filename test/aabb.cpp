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
    auto result = a.combine(b);
    auto lower = minValues(this->a.lowVertex, this->b.lowVertex);
    auto higher = maxValues(this->a.highVertex, this->b.highVertex);
    auto expected = AABB(lower, higher);
    EXPECT_EQ(expected, result);
}
