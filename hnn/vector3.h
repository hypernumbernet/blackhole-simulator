#pragma once

#include "constants.h"

namespace hnn // https://github.com/hypernumbernet
{

class Vector3
{
public:
    Vector3() {}

    Vector3(const double ax, const double ay, const double az)
        : m_x(ax), m_y(ay), m_z(az) {}

    explicit Vector3(const double* const a)
        : m_x(a[0]), m_y(a[1]), m_z(a[2]) {}

    template <typename E>
    Vector3(const double* const a, const E index)
        : m_x(a[index]), m_y(a[index + 1]), m_z(a[index + 2]) {}

    double x() const { return m_x; }
    double y() const { return m_y; }
    double z() const { return m_z; }

    void setX(const double ax) { m_x = ax; }
    void setY(const double ay) { m_y = ay; }
    void setZ(const double az) { m_z = az; }

    void set(const double ax, const double ay, const double az)
    {
        m_x = ax; m_y = ay; m_z = az;
    }

    void set(const Vector3& v)
    {
        m_x = v.m_x; m_y = v.m_y; m_z = v.m_z;
    }

    template <typename E>
    void set(const double* const a, const E i)
    {
        m_x = a[i]; m_y = a[i + 1]; m_z = a[i + 2];
    }

    const double& operator[](const int i) const
    {
        if (i == 0) return m_x;
        if (i == 1) return m_y;
        return m_z;
    }

    double& operator[](int i)
    {
        if (i == 0) return m_x;
        if (i == 1) return m_y;
        return m_z;
    }

    static Vector3 zero()
    {
        static Vector3 z(0., 0., 0.);
        return z;
    }

    const Vector3 operator+(const Vector3& a) const
    {
        return Vector3(m_x + a.m_x, m_y + a.m_y, m_z + a.m_z);
    }

    const Vector3 operator-(const Vector3& a) const
    {
        return Vector3(m_x - a.m_x, m_y - a.m_y, m_z - a.m_z);
    }

    const Vector3 operator-() const { return Vector3( - m_x, - m_y, - m_z); }

    Vector3& operator+=(const Vector3& a)
    {
        m_x += a.m_x; m_y += a.m_y; m_z += a.m_z;
        return *this;
    }

    Vector3& operator-=(const Vector3& a)
    {
        m_x -= a.m_x; m_y -= a.m_y; m_z -= a.m_z;
        return *this;
    }

    template <typename E>
    const Vector3 operator*(const E a) const
    {
        return Vector3(m_x * a, m_y * a, m_z * a);
    }

    template <typename E>
    friend const Vector3 operator*(const E a, const Vector3& b)
    {
        return Vector3(a * b.m_x, a * b.m_y, a * b.m_z);
    }

    template <typename E>
    const Vector3 operator/(const E a) const
    {
        return Vector3(m_x / a, m_y / a, m_z / a);
    }

    template <typename E>
    Vector3& operator*=(const E a)
    {
        m_x *= a; m_y *= a; m_z *= a;
        return *this;
    }

    template <typename E>
    Vector3& operator/=(const E a)
    {
        m_x /= a; m_y /= a; m_z /= a;
        return *this;
    }

    bool operator==(const Vector3& a) const
    {
        return m_x == a.m_x && m_y == a.m_y && m_z == a.m_z;
    }

    bool operator!=(const Vector3& a) const
    {
        return m_x != a.m_x || m_y != a.m_y || m_z != a.m_z;
    }

    double norm() const
    {
        return m_x * m_x + m_y * m_y + m_z * m_z;
    }

    double abs() const
    {
        return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
    }

    Vector3& normalize()
    {
        const double r = sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
        if (r != 0.)
        {
            m_x /= r; m_y /= r; m_z /= r;
        }
        return *this;
    }

    Vector3 normalized() const
    {
        const double r = sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
        if (r == 0.)
        {
            return Vector3(m_x, m_y, m_z);
        }
        return Vector3(m_x / r, m_y / r, m_z / r);
    }

    double dot(const Vector3& a) const
    {
        return m_x * a.m_x + m_y * a.m_y + m_z * a.m_z;
    }

    double distance(const Vector3& a) const
    {
        const double d1 = m_x - a.m_x;
        const double d2 = m_y - a.m_y;
        const double d3 = m_z - a.m_z;
        return sqrt(d1 * d1 + d2 * d2 + d3 * d3);
    }

    Vector3 cross(const Vector3& a) const
    {
        return Vector3(m_y * a.m_z - m_z * a.m_y, m_z * a.m_x - m_x * a.m_z, m_x * a.m_y - m_y * a.m_x);
    }

    bool isfinite() const
    {
        return std::isfinite(m_x) && std::isfinite(m_y) && std::isfinite(m_z);
    }

    bool fuzzyCompare(const Vector3& a)
    {
        return hnn::fuzzyCompare(m_x, a.m_x) && hnn::fuzzyCompare(m_y, a.m_y) && hnn::fuzzyCompare(m_z, a.m_z);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& a)
    {
        os << a.x() << ", " << a.y() << ", " << a.z();
        return os;
    }

private:
    double m_x;
    double m_y;
    double m_z;
};

} // namespace
