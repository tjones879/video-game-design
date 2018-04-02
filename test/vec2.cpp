#include "inc/vec2.hpp"
#include "gtest/gtest.h"
#include <cstdlib>


using testing::Types;

template <typename T>
class Vec2Test : public ::testing::Test {
protected:
    Vec2<T> vec1;
    Vec2<T> vec2;

    Vec2Test()
    {
        std::srand(std::time(nullptr));
        vec1.x = std::rand();
        vec1.y = std::rand();
        vec2.x = std::rand();
        vec2.y = std::rand();
    }
};

typedef Types<int, uint32_t, int32_t, float, double> implementations;

TYPED_TEST_CASE(Vec2Test, implementations);

TYPED_TEST(Vec2Test, SupportsAddition)
{
    auto result = this->vec1 + this->vec2;
    EXPECT_EQ(this->vec1.x + this->vec2.x, result.x);
    EXPECT_EQ(this->vec1.y + this->vec2.y, result.y);
}

TYPED_TEST(Vec2Test, SupportsScalarMultiplication)
{
    auto tmp = 10;
    auto result = this->vec1 * tmp;
    EXPECT_EQ(tmp * this->vec1.x, result.x);
    EXPECT_EQ(tmp * this->vec1.y, result.y);
}

TYPED_TEST(Vec2Test, SupportsSubtraction)
{
    auto result = this->vec1 - this->vec2;
    EXPECT_EQ(this->vec1.x - this->vec2.x, result.x);
    EXPECT_EQ(this->vec1.y - this->vec2.y, result.y);
}

TYPED_TEST(Vec2Test, SupportsDivision)
{
    auto result = this->vec1 / this->vec2;
    EXPECT_EQ(this->vec1.x / this->vec2.x, result.x);
    EXPECT_EQ(this->vec1.y / this->vec2.y, result.y);
}

TYPED_TEST(Vec2Test, SupportsCrossVectorProduct)
{
    auto expected = this->vec1.x * this->vec2.y - this->vec1.y * this->vec2.x;
    EXPECT_EQ(expected, cross(this->vec1, this->vec2));
}

TYPED_TEST(Vec2Test, SupportsCrossScalarProduct)
{
    const auto scalar = 5;
    auto result = cross(this->vec1, scalar);
    EXPECT_EQ(scalar * this->vec1.y, result.x);
    EXPECT_EQ(-scalar * this->vec1.x, result.y);
}

TYPED_TEST(Vec2Test, SupportsDotProduct)
{
    auto result = dot(this->vec1, this->vec2);
    auto expected = this->vec1.x * this->vec2.x + this->vec1.y * this->vec2.y;
    EXPECT_EQ(expected, result);
}
