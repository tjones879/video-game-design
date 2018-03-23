#include "gtest/gtest.h"

#include "inc/physics/collisions.hpp"
#include "inc/physics/common.hpp"

using namespace phy;
using namespace std;

TEST(CollidePolygonTest, ShouldFindCollisions)
{
    auto polyA = PolygonShape(5);
    polyA.setBox({25, 25});
    auto transformA = Transform({{50, 50}, 0});
    auto polyB = PolygonShape(5);
    polyB.setBox({25, 25});
    auto transformB = Transform({{47, 47}, 0});

    auto manifold = collidePolygons(polyA, transformA,
                                    polyB, transformB);

    EXPECT_EQ(Manifold::Type::polygons, manifold.type);
}

TEST(CollidePolygonTest, ShoudFindNonCollisions)
{
    auto polyA = PolygonShape(5);
    polyA.setBox({10, 10});
    auto transformA = Transform({{50, 50}, 0});
    auto polyB = PolygonShape(5);
    polyB.setBox({10, 10});
    auto transformB = Transform({{0, 0}, 0});

    auto manifold = collidePolygons(polyA, transformA,
                                    polyB, transformB);
    EXPECT_EQ(Manifold::Type::INVALID, manifold.type);
}
