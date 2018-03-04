#pragma once

#include <cstdint>
#include <math.h>
#include <ostream>

namespace phy {

class Vec2 {
public:
    float x, y;
    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float x, float y) : x(x), y(y) {}

    void zeroOut();
    void setVec(float x1, float y1);

    /**
     * Access a value by 0-based index.
     * @param i Index of the value that is wanted, must be 0 or 1.
     */
    float operator() (int i) const;
    void operator+= (const Vec2 &vec);
    Vec2 operator+ (const Vec2 &vec) const;
    void operator-= (const Vec2 &vec);
    void operator*= (float scalar);
    Vec2 operator* (float scalar) const;
    Vec2 operator-(const Vec2& b) const;
    /**
     * Cross product of two vectors.
     */
    float cross(const Vec2 &b) const;
    /**
     * Cross product with a scalar. This returns a Vec2 in 2 dimensions.
     */
    Vec2 cross(float s) const;
    /**
     * Calculate the dot product with another vector.
     */
    float dot(const Vec2 &vec) const;
    /**
     * Calculate the absolute magnitude of the vector with
     * the sum of squares.
     */
    float length() const;
    float normalize();
};

std::ostream &operator<<(std::ostream &out, const Vec2 v);

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
};

/**
 * Represent the transformation of a Vec2 with a
 *   linear translation,
 *   rotation around a local origin.
 */
struct Transform {
    Vec2 position;
    Rotation rotation;
    Transform(Vec2 p, Rotation r)
        : position(p), rotation(r) {}

    /**
     * Rotate a point then apply a linear translation.
     */
    Vec2 translate(const Vec2 &point) const
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

    Vec2 localCenter; ///< Local center of mass position
    Vec2 center0, center;
    float angle0, angle;
    float currentTime; ///< Current time step in range [0, 1]
};

} /* namespace phy */
