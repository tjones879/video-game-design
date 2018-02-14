#include "inc/physics/circle.hpp"

namespace phy {
CircleShape::CircleShape()
{

}

bool CircleShape::testPoint() const
{
    return true;
}

void CircleShape::getAABB() const
{

}

MassProperties CircleShape::getMassProps() const
{
    return MassProperties();
}
} /* namespace phy */
