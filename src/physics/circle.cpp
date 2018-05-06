#include "inc/physics/circle.hpp"

namespace phy {
CircleShape::CircleShape()
    : density(0.0), radius(0.0)
{
    shapeType = ShapeType::circle;
}

CircleShape::CircleShape(float dens, float rad)
    : density(dens), radius(rad), pos({0, 0}) {}

CircleShape::CircleShape(float dens, float rad, Vec2f position)
    : density(dens), radius(rad), pos(position)
{
    shapeType = ShapeType::circle;
}

CircleShape::CircleShape(const CircleShape &other)
{
    shapeType = ShapeType::circle;
    density = other.density;
    radius = other.radius;
    pos = other.pos;
}

bool CircleShape::testPoint(const Transform &transform, const Vec2f &pos) const
{
    Vec2f center = transform.position + transform.rotation.rotate(pos);
    Vec2f distance = pos - center;
    return distance.length() <= radius * radius;
}

AABB CircleShape::getAABB(const Transform &transform) const
{
    AABB aabb;
    aabb.lowVertex.setVec(pos.x - radius, pos.y - radius);
    aabb.highVertex.setVec(pos.x + radius, pos.y + radius);
    return aabb;
}

MassProperties CircleShape::getMassProps() const
{
    MassProperties data;
    data.centroid = pos;
    data.mass = density * M_PI * radius * radius;
    data.inertia = data.mass * (0.5f * radius * radius + pos.length());
    return data;
}

void CircleShape::print(std::ostream &out) const
{
    out << "Circle: " << pos << " r: " << radius << std::endl;
}
} /* namespace phy */
