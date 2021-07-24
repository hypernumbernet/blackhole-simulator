#pragma once

#include <cmath>
#include "constants.h"

namespace hnn // https://github.com/hypernumbernet
{

template <typename T>
class Vector3
{
public:
    Vector3() {}

    constexpr Vector3(const T ax, const T ay, const T az)
        : m_x(ax), m_y(ay), m_z(az) {}

    constexpr explicit Vector3(const T* const a)
        : m_x(a[0]), m_y(a[1]), m_z(a[2]) {}

    template <typename E>
    constexpr Vector3(const T* const a, const E index)
        : m_x(a[index]), m_y(a[index + 1]), m_z(a[index + 2]) {}

    constexpr T x() const { return m_x; }
    constexpr T y() const { return m_y; }
    constexpr T z() const { return m_z; }

    void setX(T ax) { m_x = ax; }
    void setY(T ay) { m_y = ay; }
    void setZ(T az) { m_z = az; }

    void set(T ax, T ay, T az)
    {
        m_x = ax;
        m_y = ay;
        m_z = az;
    }

    constexpr const Vector3 operator+(const Vector3& a) const
    {
        return Vector3(m_x + a.m_x, m_y + a.m_y, m_z + a.m_z);
    }

    constexpr const Vector3 operator-(const Vector3& a) const
    {
        return Vector3(m_x - a.m_x, m_y - a.m_y, m_z - a.m_z);
    }

    constexpr const Vector3 operator+() const { return *this; }

    constexpr const Vector3 operator-() const { return Vector3( - m_x, - m_y, - m_z); }

    Vector3& operator+=(const Vector3& a)
    {
        m_x += a.m_x;
        m_y += a.m_y;
        m_z += a.m_z;
        return *this;
    }

    Vector3& operator-=(const Vector3& a)
    {
        m_x -= a.m_x;
        m_y -= a.m_y;
        m_z -= a.m_z;
        return *this;
    }

    template <typename E>
    constexpr const Vector3 operator*(E a) const
    {
        return Vector3(m_x * a, m_y * a, m_z * a);
    }

    template <typename E>
    constexpr friend inline const Vector3 operator*(E a, const Vector3& b)
    {
        return Vector3(a * b.x(), a * b.y(), a * b.z());
    }

    template <typename E>
    Vector3& operator*=(E a)
    {
        m_x *= a;
        m_y *= a;
        m_z *= a;
        return *this;
    }

    template <typename E>
    Vector3& operator/=(E a)
    {
        m_x /= a;
        m_y /= a;
        m_z /= a;
        return *this;
    }

    constexpr bool operator==(const Vector3& a) const
    {
        return m_x == a.m_x && m_y == a.m_y && m_z == a.m_z;
    }

    constexpr bool operator!=(const Vector3& a) const
    {
        return m_x != a.m_x || m_y != a.m_y || m_z != a.m_z;
    }

    constexpr T norm() const
    {
        return m_x * m_x + m_y * m_y + m_z * m_z;
    }

    constexpr T abs() const
    {
        return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
    }

    Vector3& normalize()
    {
        T r = sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
        if (r != 0.0)
        {
            m_x /= r;
            m_y /= r;
            m_z /= r;
        }
        return *this;
    }

    Vector3 normalized() const
    {
        T r = sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
        if (r == 0.0)
            return Vector3(m_x, m_y, m_z);
        return Vector3(m_x / r, m_y / r, m_z / r);
    }

    constexpr T dot(const Vector3& a) const
    {
        return m_x * a.m_x + m_y * a.m_y + m_z * a.m_z;
    }

    constexpr T distance(const Vector3& a) const
    {
        T d1 = m_x - a.m_x;
        T d2 = m_y - a.m_y;
        T d3 = m_z - a.m_z;
        return sqrt(d1 * d1 + d2 * d2 + d3 * d3);
    }

    constexpr Vector3 cross(const Vector3& a) const
    {
        return Vector3(m_y * a.m_z - m_z * a.m_y, m_z * a.m_x - m_x * a.m_z, m_x * a.m_y - m_y * a.m_x);
    }

private:
    T m_x, m_y, m_z;
};

} // namespace
