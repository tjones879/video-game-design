#include "inc/physics/circle.hpp"

namespace phy {
CircleShape::CircleShape()
    : density(0.0), radius(0.0)
{
    shapeType = ShapeType::circle;
}

bool CircleShape::testPoint(const Transform &transform, const Vec2f &pos) const
{
    Vec2f center = transform.position + transform.rotation.rotate(pos);
    Vec2f distance = pos - center;
    return distance.length() <= radius * radius;
}

void CircleShape::getAABB() const
{

}

MassProperties CircleShape::getMassProps() const
{
    MassProperties data;
    data.centroid = pos;
    data.mass = density * M_PI * radius * radius;
    data.inertia = data.mass * (0.5f * radius * radius + pos.length());
    return data;
}
} /* namespace phy */
