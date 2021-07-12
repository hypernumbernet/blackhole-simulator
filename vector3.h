#pragma once

#include <math.h>

namespace bhs
{

template <typename TYPE>
class Vector3
{
    using T = TYPE;

public:
    union
    {
        struct
        {
            T x;
            T y;
            T z;
        };
        T array[3];
    };

    inline Vector3(){}

    inline Vector3(T x_in, T y_in, T z_in)
        : x(x_in), y(y_in), z(z_in){}

    inline explicit Vector3(const T* const a)
        : x(a[0]), y(a[1]), z(a[2]){}

    template <class E>
    inline Vector3(const T* const a, const E index)
        : x(a[index]), y(a[index + 1]), z(a[index + 2]){}

    inline void set(T x_in, T y_in, T z_in)
    {
        x = x_in;
        y = y_in;
        z = z_in;
    }

    inline Vector3 operator +(const Vector3& a) const
    {
        return Vector3(x + a.x, y + a.y, z + a.z);
    }

    inline Vector3 operator -(const Vector3& a) const
    {
        return Vector3(x - a.x, y - a.y, z - a.z);
    }

    inline Vector3 operator +() const{ return *this; }
    inline Vector3 operator -() const{ return Vector3( - x, - y, - z); }

    template <typename X>
    inline Vector3 operator *(X a) const
    {
        return Vector3(x * a, y * a, z * a);
    }

    template <typename X>
    inline friend Vector3 operator *(X a, const Vector3& b)
    {
        return Vector3(a * b.x, a * b.y, a * b.z);
    }

    inline Vector3& operator +=(const Vector3& a)
    {
        x += a.x;
        y += a.y;
        z += a.z;
        return *this;
    }

    template <typename X>
    inline Vector3& operator *=(X a)
    {
        x *= a;
        y *= a;
        z *= a;
        return *this;
    }

    inline bool operator ==(const Vector3& a) const
    {
        return ((x == a.x) && (y == a.y) && (z == a.z));
    }

    inline T Norm() const
    {
        return x * x + y * y + z * z;
    }

    inline T Abs() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    inline Vector3& Normalize()
    {
        T r = sqrt(x * x + y * y + z * z);
        if (r != 0.0)
        {
            x /= r;
            y /= r;
            z /= r;
        }
        return *this;
    }

    inline T Dot(const Vector3& a) const
    {
        return x * a.x + y * a.y + z * a.z;
    }

    inline T Distance(const Vector3& a) const
    {
        T d1 = x - a.x;
        T d2 = y - a.y;
        T d3 = z - a.z;
        return sqrt(d1 * d1 + d2 * d2 + d3 * d3);
    }

    inline Vector3 Cross(const Vector3& v) const
    {
        return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
};

} // namespace
