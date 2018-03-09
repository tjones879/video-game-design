#include "inc/vec2.hpp"
#include "gtest/gtest.h"

using testing::Types;

template <typename T>
class Vec2Test : public ::testing::Test {
protected:
    Vec2<T> vec1;
    Vec2<T> vec2;

    Vec2Test() : vec1(), vec2() {}
};

typedef Types<int, uint32_t, int32_t, float, double> implementations;

TYPED_TEST_CASE(Vec2Test, implementations);

TYPED_TEST(Vec2Test, SupportsAddition)
{
    this->vec1.x = 9;
    this->vec1.y = 10;
    this->vec2.x = 5;
    this->vec2.y = 7;
    auto result = this->vec1 + this->vec2;
    EXPECT_EQ(14, result.x);
    EXPECT_EQ(17, result.y);
}

TYPED_TEST(Vec2Test, SupportsScalarMultiplication)
{
    this->vec1.x = 3;
    this->vec1.y = 5;
    auto tmp = 10;
    auto result = this->vec1 * tmp;
    EXPECT_EQ(30, result.x);
    EXPECT_EQ(50, result.y);
}

TYPED_TEST(Vec2Test, SupportsSubtraction)
{
    this->vec1.x = 5;
    this->vec1.y = 10;
    this->vec2.x = 2;
    this->vec2.y = 4;
    auto result = this->vec1 - this->vec2;
    EXPECT_EQ(3, result.x);
    EXPECT_EQ(6, result.y);
}

TYPED_TEST(Vec2Test, SupportsDivision)
{
    this->vec1.x = 6;
    this->vec1.y = 10;
    this->vec2.x = 1;
    this->vec2.y = 2;
    auto result = this->vec1 / this->vec2;
    EXPECT_EQ(this->vec1.x / this->vec2.x, result.x);
    EXPECT_EQ(this->vec1.y / this->vec2.y, result.y);
}

TYPED_TEST(Vec2Test, SupportsCrossVectorProduct)
{
    this->vec1.x = 6;
    this->vec1.y = 3;
    this->vec2.x = 10;
    this->vec2.y = 5;
    auto expected = this->vec1.x * this->vec2.y - this->vec1.y * this->vec2.x;
    EXPECT_EQ(expected, cross(this->vec1, this->vec2));
}

TYPED_TEST(Vec2Test, SupportsCrossScalarProduct)
{
    this->vec1.x = 6;
    this->vec1.y = 3;
    const auto scalar = 5;
    auto result = cross(this->vec1, scalar);
    EXPECT_EQ(scalar * this->vec1.y, result.x);
    EXPECT_EQ(-scalar * this->vec1.x, result.y);
}

TYPED_TEST(Vec2Test, SupportsDotProduct)
{
    this->vec1.x = 3;
    this->vec1.y = 4;
    this->vec2.x = 6;
    this->vec2.y = 11;
    auto result = dot(this->vec1, this->vec2);
    auto expected = this->vec1.x * this->vec2.x + this->vec1.y * this->vec2.y;
    EXPECT_EQ(expected, result);
}

TYPED_TEST(Vec2Test, SupportsPlusEquals)
{
    this->vec1.x = 3;
    this->vec1.y = 4;
    this->vec2.x = 6;
    this->vec2.y = 11;
}
