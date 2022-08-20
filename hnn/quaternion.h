#pragma once

#include "constants.h"
#include "vector3.h"
#include <cmath>

namespace hnn // https://github.com/hypernumbernet
{

template <typename T>
class Quaternion
{
public:
    Quaternion() {}

    Quaternion(const T real, const T imaginary1, const T imaginary2, const T imaginary3)
        : m_re(real), m_i1(imaginary1), m_i2(imaginary2), m_i3(imaginary3) {}

    explicit Quaternion(const T a)
        : m_re(T(a)), m_i1(T(0)), m_i2(T(0)), m_i3(T(0)) {}

    explicit Quaternion(Vector3<T> v)
        : m_re(T(0)), m_i1(v.x()), m_i2(v.y()), m_i3(v.z()) {}

    template <typename E>
    Quaternion(const T* const a, const E index)
        : m_re(a[index]), m_i1(a[index + 1]), m_i2(a[index + 2]), m_i3(a[index + 3]) {}

    T re() const { return m_re; }
    T i1() const { return m_i1; }
    T i2() const { return m_i2; }
    T i3() const { return m_i3; }

    void setRe(T re) { m_re = re; }
    void setI1(T i1) { m_i1 = i1; }
    void setI2(T i2) { m_i2 = i2; }
    void setI3(T i3) { m_i3 = i3; }

    void set(T real, T imaginary1, T imaginary2, T imaginary3)
    {
        m_re = real;
        m_i1 = imaginary1;
        m_i2 = imaginary2;
        m_i3 = imaginary3;
    }

    void set(const T* const a)
    {
        m_re = a[0];
        m_i1 = a[1];
        m_i2 = a[2];
        m_i3 = a[3];
    }

    template <typename E>
    void set(const T* const a, const E index)
    {
        m_re = a[index];
        m_i1 = a[index + 1];
        m_i2 = a[index + 2];
        m_i3 = a[index + 3];
    }

    static constexpr Quaternion<T> zero()
    {
        return Quaternion<T>(T(0), T(0), T(0), T(0));
    }

    static constexpr Quaternion<T> identity()
    {
        return Quaternion<T>(T(1), T(0), T(0), T(0));
    }

    const Quaternion operator+(const Quaternion& a) const
    {
        return Quaternion(m_re + a.m_re, m_i1 + a.m_i1, m_i2 + a.m_i2, m_i3 + a.m_i3);
    }

    const Quaternion operator-(const Quaternion& a) const
    {
        return Quaternion(m_re - a.m_re, m_i1 - a.m_i1, m_i2 - a.m_i2, m_i3 - a.m_i3);
    }

    const Quaternion operator+() const { return *this; }

    const Quaternion operator-() const { return Quaternion(-m_re, -m_i1, -m_i2, -m_i3); }

    Quaternion& operator+=(const Quaternion& a)
    {
        m_re += a.m_re;
        m_i1 += a.m_i1;
        m_i2 += a.m_i2;
        m_i3 += a.m_i3;
        return *this;
    }

    Quaternion& operator-=(const Quaternion& a)
    {
        m_re -= a.m_re;
        m_i1 -= a.m_i1;
        m_i2 -= a.m_i2;
        m_i3 -= a.m_i3;
        return *this;
    }

    const Quaternion operator*(const Quaternion& a) const
    {
        return Quaternion(
            m_re * a.m_re - m_i1 * a.m_i1 - m_i2 * a.m_i2 - m_i3 * a.m_i3,
            m_re * a.m_i1 + m_i1 * a.m_re + m_i2 * a.m_i3 - m_i3 * a.m_i2,
            m_re * a.m_i2 - m_i1 * a.m_i3 + m_i2 * a.m_re + m_i3 * a.m_i1,
            m_re * a.m_i3 + m_i1 * a.m_i2 - m_i2 * a.m_i1 + m_i3 * a.m_re
        );
    }

    template <typename E>
    const Quaternion operator*(E a) const
    {
        return Quaternion(m_re * a, m_i1 * a, m_i2 * a, m_i3 * a);
    }

    template <typename E>
    friend inline const Quaternion operator*(E a, const Quaternion& b)
    {
        return Quaternion(a * b.m_re, a * b.m_i1, a * b.m_i2, a * b.m_i3);
    }

    const Quaternion operator/(const Quaternion& a) const
    {
        return (*this) * a.inversed();
    }

    template <typename E>
    friend inline const Quaternion operator/(E a, const Quaternion& b)
    {
        return a * b.inversed();
    }

    template <typename X>
    const Quaternion operator/(X a) const
    {
        return Quaternion(m_re / a, m_i1 / a, m_i2 / a, m_i3 / a);
    }

    Quaternion& operator*=(const Quaternion& a)
    {
        set(
            m_re * a.m_re - m_i1 * a.m_i1 - m_i2 * a.m_i2 - m_i3 * a.m_i3,
            m_re * a.m_i1 + m_i1 * a.m_re + m_i2 * a.m_i3 - m_i3 * a.m_i2,
            m_re * a.m_i2 - m_i1 * a.m_i3 + m_i2 * a.m_re + m_i3 * a.m_i1,
            m_re * a.m_i3 + m_i1 * a.m_i2 - m_i2 * a.m_i1 + m_i3 * a.m_re
        );
        return *this;
    }

    template <typename E>
    Quaternion& operator*=(E a)
    {
        m_re *= a;
        m_i1 *= a;
        m_i2 *= a;
        m_i3 *= a;
        return *this;
    }

    Quaternion& operator/=(const Quaternion& a)
    {
        Quaternion ans = (*this) * a.inversed();
        set(ans.m_re, ans.m_i1, ans.m_i2, ans.m_i3);
        return *this;
    }

    template <typename E>
    Quaternion& operator/=(E a)
    {
        m_re /= a;
        m_i1 /= a;
        m_i2 /= a;
        m_i3 /= a;
        return *this;
    }

    bool operator==(const Quaternion& a) const
    {
        return m_re == a.m_re && m_i1 == a.m_i1 && m_i2 == a.m_i2 && m_i3 == a.m_i3;
    }

    bool operator!=(const Quaternion& a) const
    {
        return m_re != a.m_i1 || m_i1 != a.m_i1 || m_i2 != a.m_i2 || m_i3 != a.m_i3;
    }

    bool fuzzyCompare(const Quaternion& a) const
    {
        return hnn::fuzzyCompare(m_re, a.m_re) &&
               hnn::fuzzyCompare(m_i1, a.m_i1) &&
               hnn::fuzzyCompare(m_i2, a.m_i2) &&
               hnn::fuzzyCompare(m_i3, a.m_i3);
    }

    const T& operator[](int index) const
    {
        return array[index];
    }

    T& operator[](int index)
    {
        return array[index];
    }

    T norm() const
    {
        return m_re * m_re + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3;
    }

    T abs() const
    {
        return sqrt(m_re * m_re + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
    }

    Quaternion& normalize()
    {
        return (*this) /= sqrt(m_re * m_re + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
    }

    Quaternion conjugated() const
    {
        return Quaternion(m_re, -m_i1, -m_i2, -m_i3);
    }

    Quaternion inversed() const
    {
        T norm = m_re * m_re + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3;
        return Quaternion(m_re / norm, -m_i1 / norm, -m_i2 / norm, -m_i3 / norm);
    }

    // Argument of complex
    T arg() const
    {
        return acos(m_re / sqrt(m_re * m_re + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3));
    }

    // Exponential
    Quaternion exp() const
    {
        T n = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        T e = exp(m_re);
        T a;

        if (n == 0)
            a = e;
        else
            a = e * sin(n) / n;

        return Quaternion(e * cos(n), m_i1 * a, m_i2 * a, m_i3 * a);
    }

    // Exponential - only imaginary part
    Quaternion expIm() const
    {
        T n = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        T a;

        if (n == 0)
            a = 0;
        else
            a = sin(n) / n;

        return Quaternion(cos(n), m_i1 * a, m_i2 * a, m_i3 * a);
    }

    // Exponential - only imaginary part
    static constexpr Quaternion exp(T x, T y, T z)
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
    static constexpr Quaternion exp(const Vector3<T>& v)
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
    static constexpr Quaternion exph(T x, T y, T z)
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
    Quaternion ln() const
    {
        T n = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        if (n == 0)
            return Quaternion::zero;

        n = atan2(n, m_re) / n;
        //scalar part
        //0.5 * log(a.m_re * a.m_re + a.i1 * a.i1 + a.i2 * a.i2 + a.i3 * a.i3)
        return Quaternion(0, m_i1 * n, m_i2 * n, m_i3 * n);
    }

    // Logarithm - Vector3
    Vector3<T> lnV3() const
    {
        T n = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        if (n == 0)
            return Vector3<T>(0, 0, 0);

        n = atan2(n, m_re) / n;
        return Vector3<T>(m_i1 * n, m_i2 * n, m_i3 * n);
    }

    Vector3<T> lnAcos() const
    {
        T n = acos(m_re);
        T s = sin(n);
        return Vector3<T>(m_i1 * n / s, m_i2 * n / s, m_i3 * n / s);
    }

    Vector3<T> lnhV3() const
    {
        T n = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        if (n == 0)
            return Vector3<T>(0, 0, 0);

        n = atanh(n) / n;
        return Vector3<T>(m_i1 * n, m_i2 * n, m_i3 * n);
    }

    Vector3<T> lnV3Half() const
    {
        T v = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        if (v == 0)
            return Vector3<T>(0, 0, 0);

        T a;
        if (m_re == 0) // v == 1
            a = PI / (T)2;
        else
            a = atan(v / m_re) / v;
        return Vector3<T>(m_i1 * a, m_i2 * a, m_i3 * a);
    }

    /*
      To shorten Quaternion rotation effecting axis calculation

      rot.conjugated() * axis * rot
      = (a - ib - jc - kd) * (ix + jy + kz) * (a + ib + jc + kd)
      = (iax + jay + kaz - iibx - ijby - ikbz - jicx - jjcy - jkcz - kidx - kjdy - kkdz) * (a + ib + jc + kd)
      = (iax + jay + kaz +   bx -  kby +  jbz +  kcx +   cy -  icz -  jdx +  idy +   dz) * (a + ib + jc + kd)
      = (bx + cy + dz + i(ax - cz + dy) + j(ay + bz - dx) + k(az - by + cx)) * (a + ib + jc + kd)
      = (bx + cy + dz)a - (ax - cz + dy)b - (ay + bz - dx)c - (az - by + cx)d
        + i((bx + cy + dz)b + (ax - cz + dy)a + (ay + bz - dx)d - (az - by + cx)c)
        + j((bx + cy + dz)c - (ax - cz + dy)d + (ay + bz - dx)a + (az - by + cx)b)
        + k((bx + cy + dz)d + (ax - cz + dy)c - (ay + bz - dx)b + (az - by + cx)a)
      = abx + acy + adz - abx + bcz - bdy - acy - bcz + cdx - adz + bdy - cdx
        + i(bbx + bcy + bdz + aax - acz + ady + ady + bdz - ddx - acz + bcy - ccx)
        + j(bcx + ccy + cdz - adx + cdz - ddy + aay + abz - adx + abz - bby + bcx)
        + k(bdx + cdy + ddz + acx - ccz + cdy - aby - bbz + bdx + aaz - aby + acx)
      =   i(bbx + 2bcy + 2bdz + aax - 2acz + 2ady - ddx - ccx)
        + j(2bcx + ccy + 2cdz - 2adx - ddy + aay + 2abz - bby)
        + k(2bdx + 2cdy + ddz + 2acx - ccz - 2aby - bbz + aaz)
      =   i((aa + bb - cc - dd)x + 2(bcy + bdz - acz + ady))
        + j((aa - bb + cc - dd)y + 2(bcx + cdz - adx + abz))
        + k((aa - bb - cc + dd)z + 2(bdx + cdy + acx - aby))
      =   i((aa + bb - cc - dd)x + 2((bc + ad)y + (bd - ac)z))
        + j((aa - bb + cc - dd)y + 2((bc - ad)x + (cd + ab)z))
        + k((aa - bb - cc + dd)z + 2((bd + ac)x + (cd - ab)y))
    */
    static constexpr void rotate(Vector3<T>& axis, const Quaternion<T>& rot)
    {
        T a = rot.re();
        T b = rot.i1();
        T c = rot.i2();
        T d = rot.i3();
        T x = axis.x();
        T y = axis.y();
        T z = axis.z();

        T aa = a*a, bb = b*b, cc = c*c, dd = d*d;
        T rx = (aa + bb - cc - dd) * x + 2.0f * ((b * c + a * d) * y + (b * d - a * c) * z);
        T ry = (aa - bb + cc - dd) * y + 2.0f * ((b * c - a * d) * x + (c * d + a * b) * z);
        T rz = (aa - bb - cc + dd) * z + 2.0f * ((b * d + a * c) * x + (c * d - a * b) * y);

        axis.setX(rx);
        axis.setY(ry);
        axis.setZ(rz);

        axis.normalize();
    }

    Quaternion rot() const
    {
        T c1 = cos(m_i1);
        T c2 = cos(m_i2);
        T c3 = cos(m_i3);
        T s1 = sin(m_i1);
        T s2 = sin(m_i2);
        T s3 = sin(m_i3);
        return Quaternion(
            c1 * c2 * c3 - s1 * s2 * s3,
            s1 * c2 * c3 + c1 * s2 * s3,
            c1 * s2 * c3 - s1 * c2 * s3,
            s1 * s2 * c3 + c1 * c2 * s3
        );
    }

    static constexpr Quaternion rot(T x, T y, T z)
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

    static constexpr Quaternion roth(T x, T y, T z)
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
    T dot(const Quaternion& a) const
    {
        return m_re * a.m_re + m_i1 * a.m_i1 + m_i2 * a.m_i2 + m_i3 * a.m_i3;
    }

    // Three left calculated from four right of the seven-dimensional cross product
    Quaternion cross7(const Quaternion& a) const
    {
        return Quaternion(
            0,
            -m_re * a.m_i1 + m_i1 * a.m_re - m_i2 * a.m_i3 + m_i3 * a.m_i2,
            -m_re * a.m_i2 + m_i1 * a.m_i3 + m_i2 * a.m_re - m_i3 * a.m_i1,
            -m_re * a.m_i3 - m_i1 * a.m_i2 + m_i2 * a.m_i1 + m_i3 * a.m_re
        );
    }

    Vector3<T> cross7V3(const Quaternion& a) const
    {
        return Vector3<T>(
            -m_re * a.m_i1 + m_i1 * a.m_re - m_i2 * a.m_i3 + m_i3 * a.m_i2,
            -m_re * a.m_i2 + m_i1 * a.m_i3 + m_i2 * a.m_re - m_i3 * a.m_i1,
            -m_re * a.m_i3 - m_i1 * a.m_i2 + m_i2 * a.m_i1 + m_i3 * a.m_re
        );
    }

    // Slerp on 3-sphere (4-dimensional)
    // The right four of the result of conjugate producting
    // the right four of the octonion with the left four of the octonion.
    // It has been omitted a lot from the octonion's 8 * 8 * 8 products.
    Quaternion rotated8(const Quaternion& rot) const
    {
        return Quaternion(
              (rot.m_re * m_re - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_re
            - (rot.m_re * m_i1 + rot.m_i1 * m_re + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i1
            - (rot.m_re * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_re + rot.m_i3 * m_i1) * rot.m_i2
            - (rot.m_re * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_re) * rot.m_i3
            ,
              (rot.m_re * m_re - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i1
            + (rot.m_re * m_i1 + rot.m_i1 * m_re + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_re
            - (rot.m_re * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_re + rot.m_i3 * m_i1) * rot.m_i3
            + (rot.m_re * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_re) * rot.m_i2
            ,
              (rot.m_re * m_re - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i2
            + (rot.m_re * m_i1 + rot.m_i1 * m_re + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i3
            + (rot.m_re * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_re + rot.m_i3 * m_i1) * rot.m_re
            - (rot.m_re * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_re) * rot.m_i1
            ,
              (rot.m_re * m_re - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i3
            - (rot.m_re * m_i1 + rot.m_i1 * m_re + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i2
            + (rot.m_re * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_re + rot.m_i3 * m_i1) * rot.m_i1
            + (rot.m_re * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_re) * rot.m_re
        );
    }

    void rotate8(const Quaternion& rot)
    {
        T q0 = (rot.m_re * m_re - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_re
             - (rot.m_re * m_i1 + rot.m_i1 * m_re + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i1
             - (rot.m_re * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_re + rot.m_i3 * m_i1) * rot.m_i2
             - (rot.m_re * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_re) * rot.m_i3;
        T q1 = (rot.m_re * m_re - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i1
             + (rot.m_re * m_i1 + rot.m_i1 * m_re + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_re
             - (rot.m_re * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_re + rot.m_i3 * m_i1) * rot.m_i3
             + (rot.m_re * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_re) * rot.m_i2;
        T q2 = (rot.m_re * m_re - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i2
             + (rot.m_re * m_i1 + rot.m_i1 * m_re + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i3
             + (rot.m_re * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_re + rot.m_i3 * m_i1) * rot.m_re
             - (rot.m_re * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_re) * rot.m_i1;
        T q3 = (rot.m_re * m_re - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i3
             - (rot.m_re * m_i1 + rot.m_i1 * m_re + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i2
             + (rot.m_re * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_re + rot.m_i3 * m_i1) * rot.m_i1
             + (rot.m_re * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_re) * rot.m_re;
        m_re = q0;
        m_i1 = q1;
        m_i2 = q2;
        m_i3 = q3;
    }

    // Slerp on 3-sphere from identity
    Quaternion rot8Identity() const
    {
        return Quaternion(
            m_re * m_re - m_i1 * m_i1 - m_i2 * m_i2 - m_i3 * m_i3,
            m_re * m_i1 + m_i1 * m_re - m_i2 * m_i3 + m_i3 * m_i2,
            m_re * m_i2 + m_i1 * m_i3 + m_i2 * m_re - m_i3 * m_i1,
            m_re * m_i3 - m_i1 * m_i2 + m_i2 * m_i1 + m_i3 * m_re
        );
    }

    // Calculate a rotating quaternion from a normalized quaternion of cross7()
    template <typename E>
    Quaternion& makeRotation(E theta)
    {
        E s = sin(theta);
        m_re = cos(theta);
        m_i1 *= s;
        m_i2 *= s;
        m_i3 *= s;
        return *this;
    }

    static constexpr Quaternion rotation(Vector3<T> v, T theta)
    {
        T c = cos(theta);
        T s = sin(theta);
        return Quaternion(c, v.x() * s, v.y() * s, v.z() * s);
    }

    static constexpr Quaternion rotation(T x, T y, T z, T theta)
    {
        T c = cos(theta);
        T s = sin(theta);
        return Quaternion(c, x * s, y * s, z * s);
    }

    // The function that combines makeRotation() and rot8().
    // v should be normalized
    Quaternion rotMove(const Vector3<T>& v, const T theta) const
    {
        T c = cos(theta);
        T s = sin(theta);
        return Quaternion(
            m_re * c - (v.x() * m_i1 + v.y() * m_i2 + v.z() * m_i3) * s,
            m_i1 * c + (v.x() * m_re + v.y() * m_i3 - v.z() * m_i2) * s,
            m_i2 * c + (v.y() * m_re + v.z() * m_i1 - v.x() * m_i3) * s,
            m_i3 * c + (v.z() * m_re - v.y() * m_i1 + v.x() * m_i2) * s
        );
    }

    static constexpr Quaternion slerp(
            const Vector3<T>& from, const Vector3<T>& to,
            T rate = 1.0, T threshold = 1.0)
    {
        auto cosVal = from.dot(to);
        if (cosVal >= threshold) // same direction
        {
            return Quaternion(1);
        }
        auto angle = acos(cosVal) * rate * 0.5;
        auto cross = from.cross(to);
        if (cosVal <= -threshold) // opposite direction
        {
            Vector3<T> tmp(1, 0, 0);
            cosVal = from.dot(to);
            if (cosVal >= threshold || cosVal <= -threshold)
            {
                cross = from.cross({0, 0, 1});
            } else {
                cross = from.cross(tmp);
            }
        }
        cross.normalize();
        return rotation(cross, angle);
    }

private:
    union
        {
            struct
            {
                T m_re;
                T m_i1;
                T m_i2;
                T m_i3;
            };
            T array[4];
        };
};

} // namespace
