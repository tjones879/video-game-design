#include "inc/physics/circle.hpp"

namespace phy {
CircleShape::CircleShape()
    : density(0.0), radius(0.0)
{
    shapeType = ShapeType::circle;
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
    MassProperties data;
    data.centroid = pos;
    data.mass = density * M_PI * radius * radius;
    data.inertia = data.mass * (0.5f * radius * radius + pos.dot(pos));
    return data;
}
} /* namespace phy */
