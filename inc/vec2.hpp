#pragma once

#include <cstdint>
#include <math.h>
#include <type_traits>
#include <ostream>

template <typename T>
class Vec2 {
    static_assert(std::is_arithmetic<T>::value, "Parameterized type must be arithmetic");
public:
    T x, y;
    Vec2() : x(0), y(0) {}
    Vec2(T x_, T y_) : x(x_), y(y_) {}

    void zeroOut()
    {
        x = 0;
        y = 0;
    }

    void setVec(T x1, T y1)
    {
        x = x1;
        y = y1;
    }

    /**
     * Access a value by 0-based index.
     * @param i Index of the value that is wanted, must be 0 or 1.
     */
    T operator() (int i) const
    {
        return (&x)[i];
    }

    void operator+= (const Vec2<T> &vec)
    {
        x += vec.x;
        y += vec.y;
    }

    void operator-= (const Vec2<T> &vec)
    {
        x -= vec.x;
        y -= vec.y;
    }

    void operator*= (T scalar)
    {
        x *= scalar;
        y *= scalar;
    }

    /**
     * Calculate the absolute magnitude of the vector with
     * the sum of squares.
     */
    T length() const
    {
        return x * x + y * y;
    }

    Vec2<float> normalize()
    {
        const float eps = 0.001f;
        Vec2<float> normalized;
        T length = this->length();
        // Assume the vector length is zero if it is too small
        if (length > eps) {
            float inverse = 1.f / length;
            normalized.x = x * inverse;
            normalized.y = y * inverse;
        }

        return normalized;
    }

    /**
     * Test whether the Vec2 a is below Vec2 b.
     *
     * @return True iff the directions of a are <= the directions of b.
     */
    template <typename T1>
    bool below(const Vec2<T1> &b) const
    {
        return (x <= b.x) && (y <= b.y);
    }

    /**
     * Test whether the Vec2 a is above Vec2 b.
     *
     * @return True iff the directions of a are >= the directions of b.
     */
    template <typename T1>
    bool above(const Vec2<T1> &b) const
    {
        return (x >= b.x) && (y >= b.y);
    }
};

template <typename T1, typename T2>
auto operator+ (const Vec2<T1> &v1, const Vec2<T2> &v2) -> Vec2<decltype(T1{} + T2{})>
{
    return Vec2<decltype(T1{} + T2{})>(v1.x + v2.x, v1.y + v2.y);
}

template <typename T1, typename T2>
auto operator* (const Vec2<T1> &vec, T2 scalar) -> Vec2<decltype(T1{} * T2{})>
{
    static_assert(std::is_arithmetic<T2>::value, "Parameterized type must be arithmetic");
    return Vec2<decltype(T1{} * T2{})>(vec.x * scalar, vec.y * scalar);
}

template <typename T1, typename T2>
auto operator* (T2 scalar, const Vec2<T1> &vec) -> Vec2<decltype(T1{} * T2{})>
{
    return vec * scalar;
}

template <typename T1, typename T2>
auto operator- (const Vec2<T1> &a, const Vec2<T2> &b) -> Vec2<decltype(T1{} - T2{})>
{
    return Vec2<decltype(T1{} - T2{})>(a.x - b.x, a.y - b.y);
}

template <typename T1, typename T2>
auto operator/ (const Vec2<T1> &a, const Vec2<T2> &b) -> Vec2<decltype(T1{} / T2{})>
{
    return Vec2<decltype(T1{} / T2{})>(a.x / b.x, a.y / b.y);
}

template <typename T>
bool operator== (const Vec2<T> &a, const Vec2<T> &b)
{
    return (a.x == b.x) && (a.y == b.y);
}

/**
 * Calculate the minimum values for each direction in the Vec2.
 *
 * @return Returns the minimum x and minimum y in the form of a Vec2<T>.
 */
template <typename T>
Vec2<T> minValues(const Vec2<T>& a, const Vec2<T>& b)
{
    return Vec2<T>(std::min(a.x, b.x), std::min(a.y, b.y));
}

/**
 * Calculate the maximum values for each direction in the Vec2.
 *
 * @return Returns the maximum x and maximum y in the form of a Vec2<T>.
 */
template <typename T>
Vec2<T> maxValues(const Vec2<T>& a, const Vec2<T>& b)
{
    return Vec2<T>(std::max(a.x, b.x), std::max(a.y, b.y));
}

/**
 * Cross product of two vectors.
 */
template <typename T1, typename T2>
auto cross(const Vec2<T1> &a, const Vec2<T2> &b) -> decltype(T1{} * T2{})
{
    return a.x * b.y - a.y * b.x;
}

/**
 * Cross product with a scalar. This returns a Vec2 in 2 dimensions.
 */
template <typename T1, typename T2>
auto cross(const Vec2<T1> &a, T2 s) -> Vec2<decltype(T1{} * T2{})>
{
    static_assert(std::is_arithmetic<T2>::value, "Parameterized type must be arithmetic");
    return Vec2<decltype(T1{} * T2{})>(s * a.y, -s * a.x);
}

/**
 * Calculate the dot product with another vector.
 */
template <typename T1, typename T2>
auto dot(const Vec2<T1> &a, const Vec2<T2> &b) -> decltype(T1{} * T2{})
{
    return a.x * b.x + a.y * b.y;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const Vec2<T> v)
{
    out << "<" << v.x << ", " << v.y << ">";
    return out;
}
