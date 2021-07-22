#pragma once

#include "vector3.h"

namespace hnn // https://github.com/hypernumbernet
{

template <typename TYPE>
class Quaternion
{
    using T = TYPE;

public:
    union
    {
        struct
        {
            T i0;
            T i1;
            T i2;
            T i3;
        };
        T array[4];
    };

    constexpr Quaternion(){}

    //inline Quaternion(T real)
    //    : i0(real){}

    inline Quaternion(T real, T imaginary1, T imaginary2, T imaginary3)
        : i0(real), i1(imaginary1), i2(imaginary2), i3(imaginary3){}

    inline explicit Quaternion(const T* const a)
        : i0(a[0]), i1(a[1]), i2(a[2]), i3(a[3]){}

    inline explicit Quaternion(Vector3<T> v)
        : i0(0), i1(v.x), i2(v.y), i3(v.z){}

    template <class E>
    inline Quaternion(const T* const a, const E index)
        : i0(a[index]), i1(a[index + 1]), i2(a[index + 2]), i3(a[index + 3]){}

    static const Quaternion<T> Zero;
    static const Quaternion<T> Identity;

    inline void set(T real, T imaginary1, T imaginary2, T imaginary3)
    {
        i0 = real;
        i1 = imaginary1;
        i2 = imaginary2;
        i3 = imaginary3;
    }

    inline void set(const T* const a)
    {
        i0 = a[0];
        i1 = a[1];
        i2 = a[2];
        i3 = a[3];
    }

    inline Quaternion operator +(const Quaternion& a) const
    {
        return Quaternion(i0 + a.i0, i1 + a.i1, i2 + a.i2, i3 + a.i3);
    }

    inline Quaternion operator -(const Quaternion& a) const
    {
        return Quaternion(i0 - a.i0, i1 - a.i1, i2 - a.i2, i3 - a.i3);
    }

    inline Quaternion operator *(const Quaternion& a) const
    {
        return Quaternion(
            i0 * a.i0 - i1 * a.i1 - i2 * a.i2 - i3 * a.i3,
            i0 * a.i1 + i1 * a.i0 + i2 * a.i3 - i3 * a.i2,
            i0 * a.i2 - i1 * a.i3 + i2 * a.i0 + i3 * a.i1,
            i0 * a.i3 + i1 * a.i2 - i2 * a.i1 + i3 * a.i0
        );
    }

    template <typename X>
    inline Quaternion operator *(X a) const
    {
        return Quaternion(i0 * a, i1 * a, i2 * a, i3 * a);
    }

    template <typename X>
    inline friend Quaternion operator *(X a, const Quaternion& b)
    {
        return Quaternion(a * b.i0, a * b.i1, a * b.i2, a * b.i3);
    }

    inline Quaternion operator /(const Quaternion& a) const
    {
        return (*this) * a.Inverse();
    }

    template <typename X>
    inline Quaternion operator /(X a) const
    {
        return Quaternion(i0 / a, i1 / a, i2 / a, i3 / a);
    }

    template <typename X>
    inline friend Quaternion operator /(X a, const Quaternion& b)
    {
        return a * b.Inverse();
    }

    inline Quaternion operator +() const{ return *this; }
    inline Quaternion operator -() const{ return Quaternion(-i0, -i1, -i2, -i3); }

    inline Quaternion& operator +=(const Quaternion& a)
    {
        i0 += a.i0;
        i1 += a.i1;
        i2 += a.i2;
        i3 += a.i3;
        return *this;
    }

    inline Quaternion& operator -=(const Quaternion& a)
    {
        i0 -= a.i0;
        i1 -= a.i1;
        i2 -= a.i2;
        i3 -= a.i3;
        return *this;
    }

    inline Quaternion& operator *=(const Quaternion& a)
    {
        set(
            i0 * a.i0 - i1 * a.i1 - i2 * a.i2 - i3 * a.i3,
            i0 * a.i1 + i1 * a.i0 + i2 * a.i3 - i3 * a.i2,
            i0 * a.i2 - i1 * a.i3 + i2 * a.i0 + i3 * a.i1,
            i0 * a.i3 + i1 * a.i2 - i2 * a.i1 + i3 * a.i0
        );
        return *this;
    }

    template <typename X>
    inline Quaternion& operator *=(X a)
    {
        i0 *= a;
        i1 *= a;
        i2 *= a;
        i3 *= a;
        return *this;
    }

    inline Quaternion& operator /=(const Quaternion& a)
    {
        Quaternion ans = (*this) * a.Inverse();
        set(ans.i0, ans.i1, ans.i2, ans.i3);
        return *this;
    }

    template <typename X>
    inline Quaternion& operator /=(X a)
    {
        i0 /= a;
        i1 /= a;
        i2 /= a;
        i3 /= a;
        return *this;
    }

    inline bool operator ==(const Quaternion& a) const
    {
        return ((i0 == a.i0) && (i1 == a.i1) && (i2 == a.i2) && (i3 == a.i3));
    }

    inline bool operator !=(const Quaternion& a) const
    {
        return ((i0 != a.i1) || (i1 != a.i1) || (i2 != a.i2) || (i3 != a.i3));
    }

    inline T Norm() const
    {
        return i0 * i0 + i1 * i1 + i2 * i2 + i3 * i3;
    }

    inline T Abs() const
    {
        return sqrt(i0 * i0 + i1 * i1 + i2 * i2 + i3 * i3);
    }

    inline Quaternion& Normalize()
    {
        return (*this) /= sqrt(i0 * i0 + i1 * i1 + i2 * i2 + i3 * i3);
    }

    inline Quaternion Conjugated() const
    {
        return Quaternion(i0, -i1, -i2, -i3);
    }

    inline Quaternion& Inverse() const
    {
        T norm = i0 * i0 + i1 * i1 + i2 * i2 + i3 * i3;
        return Quaternion(i0 / norm, -i1 / norm, -i2 / norm, -i3 / norm);
    }

    // Argument of complex
    inline T Arg() const
    {
        return acos(i0 / sqrt(i0 * i0 + i1 * i1 + i2 * i2 + i3 * i3));
    }

    // Exponential
    inline Quaternion Exp() const
    {
        T n = sqrt(i1 * i1 + i2 * i2 + i3 * i3);
        T e = exp(i0);
        T a;

        if (n == 0)
            a = e;
        else
            a = e * sin(n) / n;

        return Quaternion(e * cos(n), i1 * a, i2 * a, i3 * a);
    }

    // Exponential - only imaginary part
    inline Quaternion ExpIm() const
    {
        T n = sqrt(i1 * i1 + i2 * i2 + i3 * i3);
        T a;

        if (n == 0)
            a = 0;
        else
            a = sin(n) / n;

        return Quaternion(cos(n), i1 * a, i2 * a, i3 * a);
    }

    // Exponential - only imaginary part
    inline static Quaternion Exp(T x, T y, T z)
    {
        T n = sqrt(x * x + y * y + z * z);
        T a;

        if (n == 0)
            a = 0;
        else
            a = sin(n) / n;

        return Quaternion(cos(n), x * a, y * a, z * a);
    }

    // Exponential - only imaginary part
    inline static Quaternion Exp(const Vector3<T>& v)
    {
        T n = sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
        T a;

        if (n == 0)
            a = 0;
        else
            a = sin(n) / n;

        return Quaternion(cos(n), v.x() * a, v.y() * a, v.z() * a);
    }

    // Exponential - Hyperbolic
    inline static Quaternion Exph(T x, T y, T z)
    {
        T n = sqrt(x * x + y * y + z * z);
        T a;

        if (n == 0)
            a = 0;
        else
            a = sinh(n) / n;

        return Quaternion(cosh(n), x * a, y * a, z * a);
    }

    // Logarithm
    // Use after normalizing.
    inline Quaternion Ln() const
    {
        T n = sqrt(i1 * i1 + i2 * i2 + i3 * i3);
        if (n == 0)
            return Quaternion::Zero;

        n = atan2(n, i0) / n;
        //scalar part
        //0.5 * log(a.i0 * a.i0 + a.i1 * a.i1 + a.i2 * a.i2 + a.i3 * a.i3)
        return Quaternion(0, i1 * n, i2 * n, i3 * n);
    }

    // Logarithm - Vector3
    inline Vector3<T> LnV3() const
    {
        T n = sqrt(i1 * i1 + i2 * i2 + i3 * i3);
        if (n == 0)
            return Vector3<T>(0, 0, 0);

        n = atan2(n, i0) / n;
        return Vector3<T>(i1 * n, i2 * n, i3 * n);
    }

    inline Vector3<T> LnhV3() const
    {
        T n = sqrt(i1 * i1 + i2 * i2 + i3 * i3);
        if (n == 0)
            return Vector3<T>(0, 0, 0);

        n = atanh(n) / n;
        return Vector3<T>(i1 * n, i2 * n, i3 * n);
    }

    inline Vector3<T> LnV3Half() const
    {
        T v = sqrt(i1 * i1 + i2 * i2 + i3 * i3);
        if (v == 0)
            return Vector3<T>(0, 0, 0);

        T a;
        if (i0 == 0) // v == 1
            a = PI / (T)2;
        else
            a = atan(v / i0) / v;
        return Vector3<T>(i1 * a, i2 * a, i3 * a);
    }

    inline Quaternion Rot() const
    {
        T c1 = cos(i1);
        T c2 = cos(i2);
        T c3 = cos(i3);
        T s1 = sin(i1);
        T s2 = sin(i2);
        T s3 = sin(i3);
        return Quaternion(
            c1 * c2 * c3 - s1 * s2 * s3,
            s1 * c2 * c3 + c1 * s2 * s3,
            c1 * s2 * c3 - s1 * c2 * s3,
            s1 * s2 * c3 + c1 * c2 * s3
        );
    }

    inline static Quaternion Rot(T x, T y, T z)
    {
        T c1 = cos(x);
        T c2 = cos(y);
        T c3 = cos(z);
        T s1 = sin(x);
        T s2 = sin(y);
        T s3 = sin(z);
        return Quaternion(
            c1 * c2 * c3 - s1 * s2 * s3,
            s1 * c2 * c3 + c1 * s2 * s3,
            c1 * s2 * c3 - s1 * c2 * s3,
            s1 * s2 * c3 + c1 * c2 * s3
        );
    }

    inline static Quaternion Roth(T x, T y, T z)
    {
        T c1 = cosh(x);
        T c2 = cosh(y);
        T c3 = cosh(z);
        T s1 = sinh(x);
        T s2 = sinh(y);
        T s3 = sinh(z);
        return Quaternion(
            c1 * c2 * c3 - s1 * s2 * s3,
            s1 * c2 * c3 + c1 * s2 * s3,
            c1 * s2 * c3 - s1 * c2 * s3,
            s1 * s2 * c3 + c1 * c2 * s3
        );
    }

    // Dot product
    inline T Dot(const Quaternion& a) const
    {
        return i0 * a.i0 + i1 * a.i1 + i2 * a.i2 + i3 * a.i3;
    }

    // Three left calculated from four right of the seven-dimensional cross product
    inline Quaternion Cross7(const Quaternion& a) const
    {
        return Quaternion(
            0,
            -i0 * a.i1 + i1 * a.i0 - i2 * a.i3 + i3 * a.i2,
            -i0 * a.i2 + i1 * a.i3 + i2 * a.i0 - i3 * a.i1,
            -i0 * a.i3 - i1 * a.i2 + i2 * a.i1 + i3 * a.i0
        );
    }

    inline Vector3<T> Cross7V3(const Quaternion& a) const
    {
        return Vector3<T>(
            -i0 * a.i1 + i1 * a.i0 - i2 * a.i3 + i3 * a.i2,
            -i0 * a.i2 + i1 * a.i3 + i2 * a.i0 - i3 * a.i1,
            -i0 * a.i3 - i1 * a.i2 + i2 * a.i1 + i3 * a.i0
        );
    }

    // Slerp on 3-sphere (4-dimensional)
    // The right four of the result of conjugate producting
    // the right four of the octonion with the left four of the octonion.
    // It has been omitted a lot from the octonion's 8 * 8 * 8 products.
    inline Quaternion Rotated8(const Quaternion& rot) const
    {
        return Quaternion(
              (rot.i0 * i0 - rot.i1 * i1 - rot.i2 * i2 - rot.i3 * i3) * rot.i0
            - (rot.i0 * i1 + rot.i1 * i0 + rot.i2 * i3 - rot.i3 * i2) * rot.i1
            - (rot.i0 * i2 - rot.i1 * i3 + rot.i2 * i0 + rot.i3 * i1) * rot.i2
            - (rot.i0 * i3 + rot.i1 * i2 - rot.i2 * i1 + rot.i3 * i0) * rot.i3
            ,
              (rot.i0 * i0 - rot.i1 * i1 - rot.i2 * i2 - rot.i3 * i3) * rot.i1
            + (rot.i0 * i1 + rot.i1 * i0 + rot.i2 * i3 - rot.i3 * i2) * rot.i0
            - (rot.i0 * i2 - rot.i1 * i3 + rot.i2 * i0 + rot.i3 * i1) * rot.i3
            + (rot.i0 * i3 + rot.i1 * i2 - rot.i2 * i1 + rot.i3 * i0) * rot.i2
            ,
              (rot.i0 * i0 - rot.i1 * i1 - rot.i2 * i2 - rot.i3 * i3) * rot.i2
            + (rot.i0 * i1 + rot.i1 * i0 + rot.i2 * i3 - rot.i3 * i2) * rot.i3
            + (rot.i0 * i2 - rot.i1 * i3 + rot.i2 * i0 + rot.i3 * i1) * rot.i0
            - (rot.i0 * i3 + rot.i1 * i2 - rot.i2 * i1 + rot.i3 * i0) * rot.i1
            ,
              (rot.i0 * i0 - rot.i1 * i1 - rot.i2 * i2 - rot.i3 * i3) * rot.i3
            - (rot.i0 * i1 + rot.i1 * i0 + rot.i2 * i3 - rot.i3 * i2) * rot.i2
            + (rot.i0 * i2 - rot.i1 * i3 + rot.i2 * i0 + rot.i3 * i1) * rot.i1
            + (rot.i0 * i3 + rot.i1 * i2 - rot.i2 * i1 + rot.i3 * i0) * rot.i0
        );
    }

    inline void Rotate8(const Quaternion& rot)
    {
        T q0 = (rot.i0 * i0 - rot.i1 * i1 - rot.i2 * i2 - rot.i3 * i3) * rot.i0
             - (rot.i0 * i1 + rot.i1 * i0 + rot.i2 * i3 - rot.i3 * i2) * rot.i1
             - (rot.i0 * i2 - rot.i1 * i3 + rot.i2 * i0 + rot.i3 * i1) * rot.i2
             - (rot.i0 * i3 + rot.i1 * i2 - rot.i2 * i1 + rot.i3 * i0) * rot.i3;
        T q1 = (rot.i0 * i0 - rot.i1 * i1 - rot.i2 * i2 - rot.i3 * i3) * rot.i1
             + (rot.i0 * i1 + rot.i1 * i0 + rot.i2 * i3 - rot.i3 * i2) * rot.i0
             - (rot.i0 * i2 - rot.i1 * i3 + rot.i2 * i0 + rot.i3 * i1) * rot.i3
             + (rot.i0 * i3 + rot.i1 * i2 - rot.i2 * i1 + rot.i3 * i0) * rot.i2;
        T q2 = (rot.i0 * i0 - rot.i1 * i1 - rot.i2 * i2 - rot.i3 * i3) * rot.i2
             + (rot.i0 * i1 + rot.i1 * i0 + rot.i2 * i3 - rot.i3 * i2) * rot.i3
             + (rot.i0 * i2 - rot.i1 * i3 + rot.i2 * i0 + rot.i3 * i1) * rot.i0
             - (rot.i0 * i3 + rot.i1 * i2 - rot.i2 * i1 + rot.i3 * i0) * rot.i1;
        T q3 = (rot.i0 * i0 - rot.i1 * i1 - rot.i2 * i2 - rot.i3 * i3) * rot.i3
             - (rot.i0 * i1 + rot.i1 * i0 + rot.i2 * i3 - rot.i3 * i2) * rot.i2
             + (rot.i0 * i2 - rot.i1 * i3 + rot.i2 * i0 + rot.i3 * i1) * rot.i1
             + (rot.i0 * i3 + rot.i1 * i2 - rot.i2 * i1 + rot.i3 * i0) * rot.i0;
        i0 = q0;
        i1 = q1;
        i2 = q2;
        i3 = q3;
    }

    // Slerp on 3-sphere from identity
    inline Quaternion Rot8Identity() const
    {
        return Quaternion(
            i0 * i0 - i1 * i1 - i2 * i2 - i3 * i3,
            i0 * i1 + i1 * i0 - i2 * i3 + i3 * i2,
            i0 * i2 + i1 * i3 + i2 * i0 - i3 * i1,
            i0 * i3 - i1 * i2 + i2 * i1 + i3 * i0
        );
    }

    // Calculate a rotating quaternion from a normalized quaternion of Cross7
    template <typename X>
    inline Quaternion& MakeRotation(X theta)
    {
        X s = sin(theta);
        i0 = cos(theta);
        i1 *= s;
        i2 *= s;
        i3 *= s;
        return *this;
    }

    inline static Quaternion MakeRotation(Vector3<T> v, T theta)
    {
        T c = cos(theta);
        T s = sin(theta);
        return Quaternion(c, v.x() * s, v.y() * s, v.z() * s);
    }

    inline static Quaternion MakeRotation(T x, T y, T z, T theta)
    {
        T c = cos(theta);
        T s = sin(theta);
        return Quaternion(c, x * s, y * s, z * s);
    }

    // The function that combines MakeRotation and Rot8.
    // v should be normalized
    inline Quaternion RotMove(const T theta, const Vector3<T>& v) const
    {
        T c = cos(theta);
        T s = sin(theta);
        return Quaternion(
            i0 * c - (v.x() * i1 + v.y() * i2 + v.z() * i3) * s,
            i1 * c + (v.x() * i0 + v.y() * i3 - v.z() * i2) * s,
            i2 * c + (v.y() * i0 + v.z() * i1 - v.x() * i3) * s,
            i3 * c + (v.z() * i0 - v.y() * i1 + v.x() * i2) * s
        );
    }
};

template <typename T>
const Quaternion<T> Quaternion<T>::Zero(0, 0, 0, 0);

template <typename T>
const Quaternion<T> Quaternion<T>::Identity(1, 0, 0, 0);

} // namespace
