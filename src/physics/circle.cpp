#include "inc/physics/circle.hpp"

namespace phy {
CircleShape::CircleShape()
{
    shapeType = ShapeType::circle;
    radius = 0.0f;
}

bool CircleShape::testPoint(const Transform &transform, const Vec2 &pos) const
{
    Vec2 center = transform.position + transform.rotation.rotate(pos);
    Vec2 distance = pos - center;
    return distance.dot(distance) <= radius * radius;
}

void CircleShape::getAABB() const
{

}

MassProperties CircleShape::getMassProps() const
{
    return MassProperties();
}
} /* namespace phy */
