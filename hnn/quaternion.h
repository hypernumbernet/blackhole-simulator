#pragma once

#include "constants.h"
#include "vector3.h"

namespace hnn // https://github.com/hypernumbernet
{

class Quaternion
{
public:
    Quaternion() {}

    constexpr Quaternion(const double real, const double imaginary1, const double imaginary2, const double imaginary3)
        : m_re(real), m_i1(imaginary1), m_i2(imaginary2), m_i3(imaginary3) {}

    explicit Quaternion(const double a)
        : m_re(a), m_i1(0.0), m_i2(0.0), m_i3(0.0) {}

    explicit Quaternion(const Vector3& v)
        : m_re(0.0), m_i1(v.x()), m_i2(v.y()), m_i3(v.z()) {}

    template <typename E>
    Quaternion(const double* const a, const E index)
        : m_re(a[index]), m_i1(a[index + 1]), m_i2(a[index + 2]), m_i3(a[index + 3]) {}

    double re() const { return m_re; }
    double i1() const { return m_i1; }
    double i2() const { return m_i2; }
    double i3() const { return m_i3; }

    void setRe(const double re) { m_re = re; }
    void setI1(const double i1) { m_i1 = i1; }
    void setI2(const double i2) { m_i2 = i2; }
    void setI3(const double i3) { m_i3 = i3; }

    void set(const double real, const double imaginary1, const double imaginary2, const double imaginary3)
    {
        m_re = real;
        m_i1 = imaginary1;
        m_i2 = imaginary2;
        m_i3 = imaginary3;
    }

    void set(const Quaternion& q)
    {
        m_re = q.m_re;
        m_i1 = q.m_i1;
        m_i2 = q.m_i2;
        m_i3 = q.m_i3;
    }

    void set(const double re)
    {
        m_re = re;
        m_i1 = 0.;
        m_i2 = 0.;
        m_i3 = 0.;
    }

    void set(const double* const a)
    {
        m_re = a[0];
        m_i1 = a[1];
        m_i2 = a[2];
        m_i3 = a[3];
    }

    template <typename E>
    void set(const double* const a, const E index)
    {
        m_re = a[index];
        m_i1 = a[index + 1];
        m_i2 = a[index + 2];
        m_i3 = a[index + 3];
    }

    static constexpr Quaternion zero()
    {
        return Quaternion(0.0, 0.0, 0.0, 0.0);
    }

    static constexpr Quaternion identity()
    {
        return Quaternion(1.0, 0.0, 0.0, 0.0);
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
    const Quaternion operator*(const E a) const
    {
        return Quaternion(m_re * a, m_i1 * a, m_i2 * a, m_i3 * a);
    }

    template <typename E>
    friend const Quaternion operator*(const E a, const Quaternion& b)
    {
        return Quaternion(a * b.m_re, a * b.m_i1, a * b.m_i2, a * b.m_i3);
    }

    const Quaternion operator/(const Quaternion& a) const
    {
        return (*this) * a.inversed();
    }

    template <typename E>
    friend const Quaternion operator/(const E a, const Quaternion& b)
    {
        return a * b.inversed();
    }

    template <typename E>
    const Quaternion operator/(const E a) const
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
    Quaternion& operator*=(const E a)
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
    Quaternion& operator/=(const E a)
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

    const double& operator[](const int index) const
    {
        return array[index];
    }

    double& operator[](const int index)
    {
        return array[index];
    }

    double norm() const
    {
        return m_re * m_re + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3;
    }

    double abs() const
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
        const double norm = m_re * m_re + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3;
        return Quaternion(m_re / norm, -m_i1 / norm, -m_i2 / norm, -m_i3 / norm);
    }

    // Argument of complex
    double arg() const
    {
        //return acos(m_re / sqrt(m_re * m_re + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3));
        return atan2(sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3), m_re);
    }

    // Exponential
    Quaternion exp() const
    {
        const double n = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        const double e = std::exp(m_re);
        double a;

        if (n == 0)
            a = e;
        else
            a = e * sin(n) / n;

        return Quaternion(e * cos(n), m_i1 * a, m_i2 * a, m_i3 * a);
    }

    // Exponential - only imaginary part
    Quaternion expIm() const
    {
        const double n = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        double a;

        if (n == 0)
            a = 0;
        else
            a = sin(n) / n;

        return Quaternion(cos(n), m_i1 * a, m_i2 * a, m_i3 * a);
    }

    // Exponential - only imaginary part
    static Quaternion exp(const double x, const double y, const double z)
    {
        const double n = sqrt(x * x + y * y + z * z);
        if (n == 0.0)
            return identity();
        const double a = sin(n) / n;
        return Quaternion(cos(n), x * a, y * a, z * a);
    }

    // Exponential - only imaginary part
    static Quaternion exp(const Vector3& v)
    {
        const double n = v.abs();
        if (n == 0.0)
            return identity();
        const double a = sin(n) / n;
        return Quaternion(cos(n), v.x() * a, v.y() * a, v.z() * a);
    }

    // Logarithm
    Quaternion ln() const
    {
        double n = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        if (n == 0.0)
            return Quaternion::zero();

        n = atan2(n, m_re) / n;
        return Quaternion(log(norm()), m_i1 * n, m_i2 * n, m_i3 * n);
    }

    // Logarithm - Vector3
    Vector3 lnV3() const
    {
        double n = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        if (n == 0.0)
            return Vector3(0.0, 0.0, 0.0);

        n = atan2(n, m_re) / n;
        return Vector3(m_i1 * n, m_i2 * n, m_i3 * n);
    }

    Vector3 lnAcos() const
    {
        const double n = acos(m_re);
        const double s = sin(n);
        return Vector3(m_i1 * n / s, m_i2 * n / s, m_i3 * n / s);
    }

    Vector3 lnV3Half() const
    {
        const double v = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        if (v == 0.0)
            return Vector3(0.0, 0.0, 0.0);

        double a;
        if (m_re == 0.0) // v == 1
            a = PI * 0.5;
        else
            a = atan(v / m_re) / v;
        return Vector3(m_i1 * a, m_i2 * a, m_i3 * a);
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
    static void rotate(Vector3& axis, const Quaternion& rot)
    {
        const double a = rot.re();
        const double b = rot.i1();
        const double c = rot.i2();
        const double d = rot.i3();
        const double x = axis.x();
        const double y = axis.y();
        const double z = axis.z();

        const double aa = a * a, bb = b * b, cc = c * c, dd = d * d;
        const double rx = (aa + bb - cc - dd) * x + 2. * ((b * c + a * d) * y + (b * d - a * c) * z);
        const double ry = (aa - bb + cc - dd) * y + 2. * ((b * c - a * d) * x + (c * d + a * b) * z);
        const double rz = (aa - bb - cc + dd) * z + 2. * ((b * d + a * c) * x + (c * d - a * b) * y);

        axis.setX(rx);
        axis.setY(ry);
        axis.setZ(rz);

        axis.normalize();
    }

    Quaternion rot() const
    {
        const double c1 = cos(m_i1);
        const double c2 = cos(m_i2);
        const double c3 = cos(m_i3);
        const double s1 = sin(m_i1);
        const double s2 = sin(m_i2);
        const double s3 = sin(m_i3);
        return Quaternion(
            c1 * c2 * c3 - s1 * s2 * s3,
            s1 * c2 * c3 + c1 * s2 * s3,
            c1 * s2 * c3 - s1 * c2 * s3,
            s1 * s2 * c3 + c1 * c2 * s3
        );
    }

    static Quaternion rot(double x, double y, double z)
    {
        const double c1 = cos(x);
        const double c2 = cos(y);
        const double c3 = cos(z);
        const double s1 = sin(x);
        const double s2 = sin(y);
        const double s3 = sin(z);
        return Quaternion(
            c1 * c2 * c3 - s1 * s2 * s3,
            s1 * c2 * c3 + c1 * s2 * s3,
            c1 * s2 * c3 - s1 * c2 * s3,
            s1 * s2 * c3 + c1 * c2 * s3
        );
    }

    static Quaternion roth(double x, double y, double z)
    {
        const double c1 = cosh(x);
        const double c2 = cosh(y);
        const double c3 = cosh(z);
        const double s1 = sinh(x);
        const double s2 = sinh(y);
        const double s3 = sinh(z);
        return Quaternion(
            c1 * c2 * c3 - s1 * s2 * s3,
            s1 * c2 * c3 + c1 * s2 * s3,
            c1 * s2 * c3 - s1 * c2 * s3,
            s1 * s2 * c3 + c1 * c2 * s3
        );
    }

    // Dot product
    double dot(const Quaternion& a) const
    {
        return m_re * a.m_re + m_i1 * a.m_i1 + m_i2 * a.m_i2 + m_i3 * a.m_i3;
    }

    // Three left calculated from four right of the seven-dimensional cross product
    Quaternion cross7(const Quaternion& a) const
    {
        return Quaternion(
            0.0,
            -m_re * a.m_i1 + m_i1 * a.m_re - m_i2 * a.m_i3 + m_i3 * a.m_i2,
            -m_re * a.m_i2 + m_i1 * a.m_i3 + m_i2 * a.m_re - m_i3 * a.m_i1,
            -m_re * a.m_i3 - m_i1 * a.m_i2 + m_i2 * a.m_i1 + m_i3 * a.m_re
        );
    }

    Vector3 cross7V3(const Quaternion& a) const
    {
        return Vector3(
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
        double q0 = (rot.m_re * m_re - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_re
             - (rot.m_re * m_i1 + rot.m_i1 * m_re + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i1
             - (rot.m_re * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_re + rot.m_i3 * m_i1) * rot.m_i2
             - (rot.m_re * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_re) * rot.m_i3;
        double q1 = (rot.m_re * m_re - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i1
             + (rot.m_re * m_i1 + rot.m_i1 * m_re + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_re
             - (rot.m_re * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_re + rot.m_i3 * m_i1) * rot.m_i3
             + (rot.m_re * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_re) * rot.m_i2;
        double q2 = (rot.m_re * m_re - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i2
             + (rot.m_re * m_i1 + rot.m_i1 * m_re + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i3
             + (rot.m_re * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_re + rot.m_i3 * m_i1) * rot.m_re
             - (rot.m_re * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_re) * rot.m_i1;
        double q3 = (rot.m_re * m_re - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i3
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
    Quaternion& makeRotation(const E theta)
    {
        const E s = sin(theta);
        m_re = cos(theta);
        m_i1 *= s;
        m_i2 *= s;
        m_i3 *= s;
        return *this;
    }

    static Quaternion rotation(const Vector3& v, const double theta)
    {
        const double h = theta * 0.5;
        const double c = cos(h);
        const double s = sin(h);
        return Quaternion(c, v.x() * s, v.y() * s, v.z() * s);
    }

    static Quaternion rotation(const double x, const double y, const double z, const double theta)
    {
        const double h = theta * 0.5;
        const double c = cos(h);
        const double s = sin(h);
        return Quaternion(c, x * s, y * s, z * s);
    }

    // The function that combines makeRotation() and rot8().
    // v should be normalized
    Quaternion rotMove(const Vector3& v, const double theta) const
    {
        const double c = cos(theta);
        const double s = sin(theta);
        return Quaternion(
            m_re * c - (v.x() * m_i1 + v.y() * m_i2 + v.z() * m_i3) * s,
            m_i1 * c + (v.x() * m_re + v.y() * m_i3 - v.z() * m_i2) * s,
            m_i2 * c + (v.y() * m_re + v.z() * m_i1 - v.x() * m_i3) * s,
            m_i3 * c + (v.z() * m_re - v.y() * m_i1 + v.x() * m_i2) * s
        );
    }

    static Quaternion slerp(
            const Vector3& from, const Vector3& to,
            const double rate = 1.0, const double threshold = 1.0)
    {
        const double cosVal = from.dot(to);
        if (cosVal >= threshold) // same direction
        {
            return Quaternion(1.0);
        }
        Vector3 cross = from.cross(to);
        if (cosVal <= -threshold) // opposite direction
        {
            static Vector3 unitX(1.0, 0.0, 0.0);
            const double cosX = from.dot(unitX);
            if (cosX >= threshold || cosX <= -threshold)
            {
                cross = from.cross({0.0, 0.0, 1.0}); // on X axis
            } else {
                cross = from.cross(unitX);
            }
        }
        cross.normalize();
        const double angle = acos(cosVal) * rate;
        return rotation(cross, angle);
    }

private:
    union
        {
            struct
            {
                double m_re;
                double m_i1;
                double m_i2;
                double m_i3;
            };
            double array[4];
        };
};

inline std::ostream& operator<<(std::ostream& os, const Quaternion& qu)
{
    os << qu.re() << ", " << qu.i1() << ", " << qu.i2() << ", " << qu.i3();
    return os;
}

inline bool fuzzyCompare(const Quaternion& a, const Quaternion& b)
{
    return fuzzyCompare(a.re(), b.re()) &&
           fuzzyCompare(a.i1(), b.i1()) &&
           fuzzyCompare(a.i2(), b.i2()) &&
           fuzzyCompare(a.i3(), b.i3());
}

} // namespace
