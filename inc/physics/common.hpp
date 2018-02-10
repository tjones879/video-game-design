#pragma once

#include <cstdint>

namespace phy {

class Vec2 {
    float x, y;

    Vec2() {}
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

} /* namespace phy */
