#include "gtest/gtest.h"

#include "inc/physics/collisions.hpp"
#include "inc/physics/common.hpp"

using namespace phy;
using namespace std;

TEST(CollideCircleTest, ShouldFindCollisions)
{
    auto circleA = CircleShape(0, 10);
    Transform transformA({{50, 50}, 0});
    auto circleB = CircleShape(0, 10);
    Transform transformB({{40, 40}, 0});

    auto manifold = collideCircles(circleA, transformA,
                                   circleB, transformB);
    EXPECT_EQ(Manifold::Type::circles, manifold.type);
}

TEST(CollideCircleTest, ShouldFindNonCollisions)
{
    auto circleA = CircleShape(0, 10);
    Transform transformA({{100, 100}, 0});
    auto circleB = CircleShape(0, 10);
    Transform transformB({{40, 40}, 0});

    auto manifold = collideCircles(circleA, transformA,
                                   circleB, transformB);
    EXPECT_EQ(Manifold::Type::INVALID, manifold.type);
}

TEST(CollidePolygonTest, ShouldFindCollisions)
{
    auto polyA = PolygonShape(5);
    polyA.setBox({25, 25});
    Transform transformA({{50, 50}, 0});
    auto polyB = PolygonShape(5);
    polyB.setBox({25, 25});
    Transform transformB({{95, 50}, 0});
    float depth = 5;
    Vec2f axis{1, 0};

    auto manifold = collidePolygons(polyA, transformA,
                                    polyB, transformB);

    EXPECT_EQ(Manifold::Type::polygons, manifold.type);
    EXPECT_EQ(depth, manifold.depth);
    EXPECT_EQ(axis, manifold.localNormal);
}

TEST(CollidePolygonTest, ShoudFindNonCollisions)
{
    auto polyA = PolygonShape(5);
    polyA.setBox({10, 10});
    Transform transformA({{50, 50}, 0});
    auto polyB = PolygonShape(5);
    polyB.setBox({10, 10});
    Transform transformB({{0, 0}, 0});

    auto manifold = collidePolygons(polyA, transformA,
                                    polyB, transformB);
    EXPECT_EQ(Manifold::Type::INVALID, manifold.type);
}
