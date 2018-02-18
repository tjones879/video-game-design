#pragma once

#include <cstdint>
#include <math.h>

namespace phy {

class Vec2 {
public:
    float x, y;
    Vec2()
    {
        x = 0.f;
        y = 0.f;
    }
    Vec2(float x, float y) : x(x), y(y) {}

    void zeroOut()
    {
        x = 0.0f;
        y = 0.0f;
    }

    void setVec(float x1, float y1)
    {
        x = x1;
        y = y1;
    }

    float operator() (int i) const
    {
        return (&x)[i];
    }

    void operator+= (const Vec2 &vec)
    {
        x += vec.x;
        y += vec.y;
    }

    Vec2 operator+ (const Vec2 &vec) const
    {
        return Vec2(x + vec.x, y + vec.y);
    }

    void operator-= (const Vec2 &vec)
    {
        x -= vec.x;
        y -= vec.y;
    }

    /**
     * Scalar multiplication.
     */
    void operator*= (float scalar)
    {
        x *= scalar;
        y *= scalar;
    }

    Vec2 operator* (float scalar) const
    {
        return Vec2(x * scalar, y * scalar);
    }

    Vec2 operator-(const Vec2& b) const
    {
        return Vec2(x - b.x, y - b.y);
    }

    float cross(const Vec2 &b) const
    {
        return x * b.y - y * b.x;
    }

    Vec2 cross(float s) const
    {
        return Vec2(s * y, -s * x);
    }

    /**
     * Calculate the dot product with another vector.
     */
    float dot(const Vec2 &vec) const
    {
        return x * vec.x + y * vec.y;
    }

    float length() const
    {
        return x * x + y * y;
    }

    float normalize()
    {
        const float eps = 0.001f;
        float length = length;
        if (length < eps)
            return 0.f;

        float inverse = 1.f / length;
        x *= inverse;
        y *= inverse;

        return length;
    }
};

struct Rotation {
    Rotation() {}
    Rotation(float angle)
    {
        sine = sinf(angle);
        cosine = cosf(angle);
    }
    inline Vec2 rotate(const Vec2 &point) const
    {
        return Vec2(cosine * point.x - sine * point.y,
                    sine * point.x + cosine * point.y);
    }
    inline Vec2 invRotate(const Vec2 &point) const
    {
        return Vec2(cosine * point.x + sine * point.y,
                    -sine * point.x + cosine * point.y);
    }
    float sine, cosine;
};

struct Transform {
    Transform(Vec2 p, Rotation r)
        : position(p), rotation(r) {}
    Vec2 translate(const Vec2 &point) const
    {
        auto translated = rotation.rotate(point);
        translated.x += position.x;
        translated.y += position.y;
        return translated;
    }
    Vec2 position;
    Rotation rotation;
};

struct Sweep {
    void Step();

    Vec2 localCenter; ///< Local center of mass position
    Vec2 center0, center;
    float angle0, angle;
    float currentTime; ///< Current time step in range [0, 1]
};

} /* namespace phy */
