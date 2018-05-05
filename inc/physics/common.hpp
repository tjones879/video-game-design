#pragma once

#include <cstdint>
#include <math.h>
#include <ostream>
#include "inc/vec2.hpp"
#include <cmath>

namespace phy {

using Vec2f = Vec2<float>;

/**
 * Represent any rotation of a shape in the world.
 *
 * We simply save the sine and cosine of of an angle
 * upon construction for quick rotation of a Vec2.
 */
struct Rotation {
    float sine, cosine;
    Rotation()
        : sine(sinf(0)), cosine(cosf(0)) {}
    Rotation(float angle)
        : sine(sinf(angle)), cosine(cosf(angle)) {}

    inline Vec2f rotate(const Vec2f &point) const
    {
        return Vec2f(cosine * point.x - sine * point.y,
                     sine * point.x + cosine * point.y);
    }

    inline Vec2f invRotate(const Vec2f &point) const
    {
        return Vec2f(cosine * point.x + sine * point.y,
                     -sine * point.x + cosine * point.y);
    }
};

/**
 * Represent the transformation of a Vec2 with a
 *   linear translation,
 *   rotation around a local origin.
 */
struct Transform {
    Vec2f position;
    Rotation rotation;
    Transform(Vec2f p, Rotation r)
        : position(p), rotation(r) {}
    Transform() {}

    /**
     * Rotate a point then apply a linear translation.
     */
    inline Vec2f translate(const Vec2f &point) const
    {
        auto translated = rotation.rotate(point);
        translated.x += position.x;
        translated.y += position.y;
        return translated;
    }
};

/**
 * Store the position information for a body for iterative resolution
 * of position and velocity.
 */
struct Sweep {
    void Step();

    Vec2f localCenter; ///< Local center of mass position
    Vec2f center0, center;
    float angle0, angle;
    float currentTime; ///< Current time step in range [0, 1]
};
} // namespace phy
