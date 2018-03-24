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
    Transform transformB({{45, 50}, 0});

    auto manifold = collidePolygons(polyA, transformA,
                                    polyB, transformB);

    std::cout << manifold.penetration.first << ", " << manifold.penetration.second << std::endl;
    EXPECT_EQ(Manifold::Type::polygons, manifold.type);
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
