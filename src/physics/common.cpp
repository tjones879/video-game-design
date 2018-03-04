#include "inc/physics/common.hpp"

namespace phy {

void Vec2::zeroOut()
{
    x = 0.0f;
    y = 0.0f;
}

void Vec2::setVec(float x1, float y1)
{
    x = x1;
    y = y1;
}

float Vec2::operator() (int i) const
{
    return (&x)[i];
}

void Vec2::operator+= (const Vec2 &vec)
{
    x += vec.x;
    y += vec.y;
}

Vec2 Vec2::operator+ (const Vec2 &vec) const
{
    return Vec2(x + vec.x, y + vec.y);
}

void Vec2::operator-= (const Vec2 &vec)
{
    x -= vec.x;
    y -= vec.y;
}

void Vec2::operator*= (float scalar)
{
    x *= scalar;
    y *= scalar;
}

Vec2 Vec2::operator* (float scalar) const
{
    return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator-(const Vec2& b) const
{
    return Vec2(x - b.x, y - b.y);
}

float Vec2::cross(const Vec2 &b) const
{
    return x * b.y - y * b.x;
}

Vec2 Vec2::cross(float s) const
{
    return Vec2(s * y, -s * x);
}

float Vec2::dot(const Vec2 &vec) const
{
    return x * vec.x + y * vec.y;
}

float Vec2::length() const
{
    return x * x + y * y;
}

float Vec2::normalize()
{
    const float eps = 0.001f;
    float length = this->length();
    // Assume the vector length is zero if it is too small
    if (length < eps)
        return 0.f;

    float inverse = 1.f / length;
    x *= inverse;
    y *= inverse;

    return length;
}

std::ostream &operator<<(std::ostream &out, const Vec2 v)
{
    out << "<" << v.x << ", " << v.y << ">";
    return out;
}
} /* namespace phy */
