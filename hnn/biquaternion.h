#pragma once

#include "constants.h"
#include "vector3.h"

namespace hnn // https://github.com/hypernumbernet
{

/**

*/
class Biquaternion
{
public:
    Biquaternion() {}

    constexpr Biquaternion(const double imaginary0, const double imaginary1, const double imaginary2, const double imaginary3)
        : m_i0(imaginary0), m_i1(imaginary1), m_i2(imaginary2), m_i3(imaginary3) {}

    explicit Biquaternion(const double a)
        : m_i0(a), m_i1(0.0), m_i2(0.0), m_i3(0.0) {}

    explicit Biquaternion(const Vector3 v)
        : m_i0(0.0), m_i1(v.x()), m_i2(v.y()), m_i3(v.z()) {}

    template <typename E>
    Biquaternion(const double* const a, const E index)
        : m_i0(a[index]), m_i1(a[index + 1]), m_i2(a[index + 2]), m_i3(a[index + 3]) {}

    double re() const { return m_i0; }
    double i1() const { return m_i1; }
    double i2() const { return m_i2; }
    double i3() const { return m_i3; }

    void setRe(const double re) { m_i0 = re; }
    void setI1(const double i1) { m_i1 = i1; }
    void setI2(const double i2) { m_i2 = i2; }
    void setI3(const double i3) { m_i3 = i3; }

    void set(const double real, const double imaginary1, double imaginary2, double imaginary3)
    {
        m_i0 = real;
        m_i1 = imaginary1;
        m_i2 = imaginary2;
        m_i3 = imaginary3;
    }

    void set(const double* const a)
    {
        m_i0 = a[0];
        m_i1 = a[1];
        m_i2 = a[2];
        m_i3 = a[3];
    }

    template <typename E>
    void set(const double* const a, const E index)
    {
        m_i0 = a[index];
        m_i1 = a[index + 1];
        m_i2 = a[index + 2];
        m_i3 = a[index + 3];
    }

    static constexpr Biquaternion zero()
    {
        return Biquaternion(0.0, 0.0, 0.0, 0.0);
    }

    static constexpr Biquaternion identity()
    {
        return Biquaternion(1.0, 0.0, 0.0, 0.0);
    }

    const Biquaternion operator+(const Biquaternion& a) const
    {
        return Biquaternion(m_i0 + a.m_i0, m_i1 + a.m_i1, m_i2 + a.m_i2, m_i3 + a.m_i3);
    }

    const Biquaternion operator-(const Biquaternion& a) const
    {
        return Biquaternion(m_i0 - a.m_i0, m_i1 - a.m_i1, m_i2 - a.m_i2, m_i3 - a.m_i3);
    }

    const Biquaternion operator+() const { return *this; }

    const Biquaternion operator-() const { return Biquaternion(-m_i0, -m_i1, -m_i2, -m_i3); }

    Biquaternion& operator+=(const Biquaternion& a)
    {
        m_i0 += a.m_i0;
        m_i1 += a.m_i1;
        m_i2 += a.m_i2;
        m_i3 += a.m_i3;
        return *this;
    }

    Biquaternion& operator-=(const Biquaternion& a)
    {
        m_i0 -= a.m_i0;
        m_i1 -= a.m_i1;
        m_i2 -= a.m_i2;
        m_i3 -= a.m_i3;
        return *this;
    }

    const Biquaternion operator*(const Biquaternion& a) const
    {
        return Biquaternion(
            m_i0 * a.m_i0 - m_i1 * a.m_i1 - m_i2 * a.m_i2 - m_i3 * a.m_i3,
            m_i0 * a.m_i1 + m_i1 * a.m_i0 + m_i2 * a.m_i3 - m_i3 * a.m_i2,
            m_i0 * a.m_i2 - m_i1 * a.m_i3 + m_i2 * a.m_i0 + m_i3 * a.m_i1,
            m_i0 * a.m_i3 + m_i1 * a.m_i2 - m_i2 * a.m_i1 + m_i3 * a.m_i0
        );
    }

    template <typename E>
    const Biquaternion operator*(E a) const
    {
        return Biquaternion(m_i0 * a, m_i1 * a, m_i2 * a, m_i3 * a);
    }

    template <typename E>
    friend const Biquaternion operator*(E a, const Biquaternion& b)
    {
        return Biquaternion(a * b.m_i0, a * b.m_i1, a * b.m_i2, a * b.m_i3);
    }

    const Biquaternion operator/(const Biquaternion& a) const
    {
        return (*this) * a.inversed();
    }

    template <typename E>
    friend const Biquaternion operator/(E a, const Biquaternion& b)
    {
        return a * b.inversed();
    }

    template <typename X>
    const Biquaternion operator/(X a) const
    {
        return Biquaternion(m_i0 / a, m_i1 / a, m_i2 / a, m_i3 / a);
    }

    Biquaternion& operator*=(const Biquaternion& a)
    {
        set(
            m_i0 * a.m_i0 - m_i1 * a.m_i1 - m_i2 * a.m_i2 - m_i3 * a.m_i3,
            m_i0 * a.m_i1 + m_i1 * a.m_i0 + m_i2 * a.m_i3 - m_i3 * a.m_i2,
            m_i0 * a.m_i2 - m_i1 * a.m_i3 + m_i2 * a.m_i0 + m_i3 * a.m_i1,
            m_i0 * a.m_i3 + m_i1 * a.m_i2 - m_i2 * a.m_i1 + m_i3 * a.m_i0
        );
        return *this;
    }

    template <typename E>
    Biquaternion& operator*=(E a)
    {
        m_i0 *= a;
        m_i1 *= a;
        m_i2 *= a;
        m_i3 *= a;
        return *this;
    }

    Biquaternion& operator/=(const Biquaternion& a)
    {
        Biquaternion ans = (*this) * a.inversed();
        set(ans.m_i0, ans.m_i1, ans.m_i2, ans.m_i3);
        return *this;
    }

    template <typename E>
    Biquaternion& operator/=(E a)
    {
        m_i0 /= a;
        m_i1 /= a;
        m_i2 /= a;
        m_i3 /= a;
        return *this;
    }

    bool operator==(const Biquaternion& a) const
    {
        return m_i0 == a.m_i0 && m_i1 == a.m_i1 && m_i2 == a.m_i2 && m_i3 == a.m_i3;
    }

    bool operator!=(const Biquaternion& a) const
    {
        return m_i0 != a.m_i1 || m_i1 != a.m_i1 || m_i2 != a.m_i2 || m_i3 != a.m_i3;
    }

    bool fuzzyCompare(const Biquaternion& a) const
    {
        return hnn::fuzzyCompare(m_i0, a.m_i0) &&
               hnn::fuzzyCompare(m_i1, a.m_i1) &&
               hnn::fuzzyCompare(m_i2, a.m_i2) &&
               hnn::fuzzyCompare(m_i3, a.m_i3);
    }

    const double& operator[](int index) const
    {
        return array[index];
    }

    double& operator[](int index)
    {
        return array[index];
    }

    double norm() const
    {
        return m_i0 * m_i0 + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3;
    }

    double abs() const
    {
        return sqrt(m_i0 * m_i0 + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
    }

    Biquaternion& normalize()
    {
        return (*this) /= sqrt(m_i0 * m_i0 + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
    }

    Biquaternion conjugated() const
    {
        return Biquaternion(m_i0, -m_i1, -m_i2, -m_i3);
    }

    Biquaternion inversed() const
    {
        double norm = m_i0 * m_i0 + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3;
        return Biquaternion(m_i0 / norm, -m_i1 / norm, -m_i2 / norm, -m_i3 / norm);
    }

    // Argument of complex
    double arg() const
    {
        return acos(m_i0 / sqrt(m_i0 * m_i0 + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3));
    }

    // Exponential
    Biquaternion exp() const
    {
        double n = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        double e = std::exp(m_i0);
        double a;

        if (n == 0)
            a = e;
        else
            a = e * sin(n) / n;

        return Biquaternion(e * cos(n), m_i1 * a, m_i2 * a, m_i3 * a);
    }

    // Exponential - only imaginary part
    Biquaternion expIm() const
    {
        double n = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        double a;

        if (n == 0)
            a = 0;
        else
            a = sin(n) / n;

        return Biquaternion(cos(n), m_i1 * a, m_i2 * a, m_i3 * a);
    }

    // Exponential - only imaginary part
    static Biquaternion exp(double x, double y, double z)
    {
        double n = sqrt(x * x + y * y + z * z);
        double a;

        if (n == 0.0)
            a = 0.0;
        else
            a = sin(n) / n;

        return Biquaternion(cos(n), x * a, y * a, z * a);
    }

    // Exponential - only imaginary part
    static Biquaternion exp(const Vector3& v)
    {
        double n = sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
        double a = 0.0;

        if (n == 0.0)
            a = 0.0;
        else
            a = sin(n) / n;

        return Biquaternion(cos(n), v.x() * a, v.y() * a, v.z() * a);
    }

    // Exponential - Hyperbolic
    static Biquaternion exph(double x, double y, double z)
    {
        double n = sqrt(x * x + y * y + z * z);
        double a = 0.0;

        if (n == 0.0)
            a = 0.0;
        else
            a = sinh(n) / n;

        return Biquaternion(cosh(n), x * a, y * a, z * a);
    }

    // Logarithm
    Biquaternion ln() const
    {
        double n = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        if (n == 0.0)
            return Biquaternion::zero();

        n = atan2(n, m_i0) / n;
        return Biquaternion(log(norm()), m_i1 * n, m_i2 * n, m_i3 * n);
    }

    // Logarithm - Vector3
    Vector3 lnV3() const
    {
        double n = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        if (n == 0.0)
            return Vector3(0.0, 0.0, 0.0);

        n = atan2(n, m_i0) / n;
        return Vector3(m_i1 * n, m_i2 * n, m_i3 * n);
    }

    Vector3 lnAcos() const
    {
        double n = acos(m_i0);
        double s = sin(n);
        return Vector3(m_i1 * n / s, m_i2 * n / s, m_i3 * n / s);
    }

    Vector3 lnhV3() const
    {
        double n = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        if (n == 0.0)
            return Vector3(0.0, 0.0, 0.0);

        n = atanh(n) / n;
        return Vector3(m_i1 * n, m_i2 * n, m_i3 * n);
    }

    Vector3 lnV3Half() const
    {
        double v = sqrt(m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        if (v == 0.0)
            return Vector3(0.0, 0.0, 0.0);

        double a;
        if (m_i0 == 0.0) // v == 1
            a = PI / (double)2;
        else
            a = atan(v / m_i0) / v;
        return Vector3(m_i1 * a, m_i2 * a, m_i3 * a);
    }

    /*
      To shorten Biquaternion rotation effecting axis calculation

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
    static void rotate(Vector3& axis, const Biquaternion& rot)
    {
        double a = rot.re();
        double b = rot.i1();
        double c = rot.i2();
        double d = rot.i3();
        double x = axis.x();
        double y = axis.y();
        double z = axis.z();

        double aa = a*a, bb = b*b, cc = c*c, dd = d*d;
        double rx = (aa + bb - cc - dd) * x + 2.0f * ((b * c + a * d) * y + (b * d - a * c) * z);
        double ry = (aa - bb + cc - dd) * y + 2.0f * ((b * c - a * d) * x + (c * d + a * b) * z);
        double rz = (aa - bb - cc + dd) * z + 2.0f * ((b * d + a * c) * x + (c * d - a * b) * y);

        axis.setX(rx);
        axis.setY(ry);
        axis.setZ(rz);

        axis.normalize();
    }

    Biquaternion rot() const
    {
        double c1 = cos(m_i1);
        double c2 = cos(m_i2);
        double c3 = cos(m_i3);
        double s1 = sin(m_i1);
        double s2 = sin(m_i2);
        double s3 = sin(m_i3);
        return Biquaternion(
            c1 * c2 * c3 - s1 * s2 * s3,
            s1 * c2 * c3 + c1 * s2 * s3,
            c1 * s2 * c3 - s1 * c2 * s3,
            s1 * s2 * c3 + c1 * c2 * s3
        );
    }

    static Biquaternion rot(double x, double y, double z)
    {
        double c1 = cos(x);
        double c2 = cos(y);
        double c3 = cos(z);
        double s1 = sin(x);
        double s2 = sin(y);
        double s3 = sin(z);
        return Biquaternion(
            c1 * c2 * c3 - s1 * s2 * s3,
            s1 * c2 * c3 + c1 * s2 * s3,
            c1 * s2 * c3 - s1 * c2 * s3,
            s1 * s2 * c3 + c1 * c2 * s3
        );
    }

    static Biquaternion roth(double x, double y, double z)
    {
        double c1 = cosh(x);
        double c2 = cosh(y);
        double c3 = cosh(z);
        double s1 = sinh(x);
        double s2 = sinh(y);
        double s3 = sinh(z);
        return Biquaternion(
            c1 * c2 * c3 - s1 * s2 * s3,
            s1 * c2 * c3 + c1 * s2 * s3,
            c1 * s2 * c3 - s1 * c2 * s3,
            s1 * s2 * c3 + c1 * c2 * s3
        );
    }

    // Dot product
    double dot(const Biquaternion& a) const
    {
        return m_i0 * a.m_i0 + m_i1 * a.m_i1 + m_i2 * a.m_i2 + m_i3 * a.m_i3;
    }

    // Three left calculated from four right of the seven-dimensional cross product
    Biquaternion cross7(const Biquaternion& a) const
    {
        return Biquaternion(
            0.0,
            -m_i0 * a.m_i1 + m_i1 * a.m_i0 - m_i2 * a.m_i3 + m_i3 * a.m_i2,
            -m_i0 * a.m_i2 + m_i1 * a.m_i3 + m_i2 * a.m_i0 - m_i3 * a.m_i1,
            -m_i0 * a.m_i3 - m_i1 * a.m_i2 + m_i2 * a.m_i1 + m_i3 * a.m_i0
        );
    }

    Vector3 cross7V3(const Biquaternion& a) const
    {
        return Vector3(
            -m_i0 * a.m_i1 + m_i1 * a.m_i0 - m_i2 * a.m_i3 + m_i3 * a.m_i2,
            -m_i0 * a.m_i2 + m_i1 * a.m_i3 + m_i2 * a.m_i0 - m_i3 * a.m_i1,
            -m_i0 * a.m_i3 - m_i1 * a.m_i2 + m_i2 * a.m_i1 + m_i3 * a.m_i0
        );
    }

    // Slerp on 3-sphere (4-dimensional)
    // The right four of the result of conjugate producting
    // the right four of the octonion with the left four of the octonion.
    // It has been omitted a lot from the octonion's 8 * 8 * 8 products.
    Biquaternion rotated8(const Biquaternion& rot) const
    {
        return Biquaternion(
              (rot.m_i0 * m_i0 - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i0
            - (rot.m_i0 * m_i1 + rot.m_i1 * m_i0 + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i1
            - (rot.m_i0 * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_i0 + rot.m_i3 * m_i1) * rot.m_i2
            - (rot.m_i0 * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_i0) * rot.m_i3
            ,
              (rot.m_i0 * m_i0 - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i1
            + (rot.m_i0 * m_i1 + rot.m_i1 * m_i0 + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i0
            - (rot.m_i0 * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_i0 + rot.m_i3 * m_i1) * rot.m_i3
            + (rot.m_i0 * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_i0) * rot.m_i2
            ,
              (rot.m_i0 * m_i0 - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i2
            + (rot.m_i0 * m_i1 + rot.m_i1 * m_i0 + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i3
            + (rot.m_i0 * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_i0 + rot.m_i3 * m_i1) * rot.m_i0
            - (rot.m_i0 * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_i0) * rot.m_i1
            ,
              (rot.m_i0 * m_i0 - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i3
            - (rot.m_i0 * m_i1 + rot.m_i1 * m_i0 + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i2
            + (rot.m_i0 * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_i0 + rot.m_i3 * m_i1) * rot.m_i1
            + (rot.m_i0 * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_i0) * rot.m_i0
        );
    }

    void rotate8(const Biquaternion& rot)
    {
        double q0 = (rot.m_i0 * m_i0 - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i0
             - (rot.m_i0 * m_i1 + rot.m_i1 * m_i0 + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i1
             - (rot.m_i0 * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_i0 + rot.m_i3 * m_i1) * rot.m_i2
             - (rot.m_i0 * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_i0) * rot.m_i3;
        double q1 = (rot.m_i0 * m_i0 - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i1
             + (rot.m_i0 * m_i1 + rot.m_i1 * m_i0 + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i0
             - (rot.m_i0 * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_i0 + rot.m_i3 * m_i1) * rot.m_i3
             + (rot.m_i0 * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_i0) * rot.m_i2;
        double q2 = (rot.m_i0 * m_i0 - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i2
             + (rot.m_i0 * m_i1 + rot.m_i1 * m_i0 + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i3
             + (rot.m_i0 * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_i0 + rot.m_i3 * m_i1) * rot.m_i0
             - (rot.m_i0 * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_i0) * rot.m_i1;
        double q3 = (rot.m_i0 * m_i0 - rot.m_i1 * m_i1 - rot.m_i2 * m_i2 - rot.m_i3 * m_i3) * rot.m_i3
             - (rot.m_i0 * m_i1 + rot.m_i1 * m_i0 + rot.m_i2 * m_i3 - rot.m_i3 * m_i2) * rot.m_i2
             + (rot.m_i0 * m_i2 - rot.m_i1 * m_i3 + rot.m_i2 * m_i0 + rot.m_i3 * m_i1) * rot.m_i1
             + (rot.m_i0 * m_i3 + rot.m_i1 * m_i2 - rot.m_i2 * m_i1 + rot.m_i3 * m_i0) * rot.m_i0;
        m_i0 = q0;
        m_i1 = q1;
        m_i2 = q2;
        m_i3 = q3;
    }

    // Slerp on 3-sphere from identity
    Biquaternion rot8Identity() const
    {
        return Biquaternion(
            m_i0 * m_i0 - m_i1 * m_i1 - m_i2 * m_i2 - m_i3 * m_i3,
            m_i0 * m_i1 + m_i1 * m_i0 - m_i2 * m_i3 + m_i3 * m_i2,
            m_i0 * m_i2 + m_i1 * m_i3 + m_i2 * m_i0 - m_i3 * m_i1,
            m_i0 * m_i3 - m_i1 * m_i2 + m_i2 * m_i1 + m_i3 * m_i0
        );
    }

    // Calculate a rotating Biquaternion from a normalized Biquaternion of cross7()
    template <typename E>
    Biquaternion& makeRotation(E theta)
    {
        E s = sin(theta);
        m_i0 = cos(theta);
        m_i1 *= s;
        m_i2 *= s;
        m_i3 *= s;
        return *this;
    }

    static Biquaternion rotation(Vector3 v, double theta)
    {
        double c = cos(theta);
        double s = sin(theta);
        return Biquaternion(c, v.x() * s, v.y() * s, v.z() * s);
    }

    static Biquaternion rotation(double x, double y, double z, double theta)
    {
        double c = cos(theta);
        double s = sin(theta);
        return Biquaternion(c, x * s, y * s, z * s);
    }

    // The function that combines makeRotation() and rot8().
    // v should be normalized
    Biquaternion rotMove(const Vector3& v, const double theta) const
    {
        double c = cos(theta);
        double s = sin(theta);
        return Biquaternion(
            m_i0 * c - (v.x() * m_i1 + v.y() * m_i2 + v.z() * m_i3) * s,
            m_i1 * c + (v.x() * m_i0 + v.y() * m_i3 - v.z() * m_i2) * s,
            m_i2 * c + (v.y() * m_i0 + v.z() * m_i1 - v.x() * m_i3) * s,
            m_i3 * c + (v.z() * m_i0 - v.y() * m_i1 + v.x() * m_i2) * s
        );
    }

    static Biquaternion slerp(
            const Vector3& from, const Vector3& to,
            double rate = 1.0, double threshold = 1.0)
    {
        auto cosVal = from.dot(to);
        if (cosVal >= threshold) // same direction
        {
            return Biquaternion(1.0);
        }
        auto angle = acos(cosVal) * rate * 0.5;
        auto cross = from.cross(to);
        if (cosVal <= -threshold) // opposite direction
        {
            Vector3 tmp(1.0, 0.0, 0.0);
            cosVal = from.dot(to);
            if (cosVal >= threshold || cosVal <= -threshold)
            {
                cross = from.cross({0.0, 0.0, 1.0});
            } else {
                cross = from.cross(tmp);
            }
        }
        cross.normalize();
        return rotation(cross, angle);
    }

    std::string toString()
    {
        std::ostringstream o;
        o << m_i0 << ", " << m_i1 << ", " << m_i2 << ", " << m_i3;
        return o.str();
    }

private:
    union
        {
            struct
            {
                double m_i0;
                double m_i1;
                double m_i2;
                double m_i3;
            };
            double array[4];
        };
};

} // namespace
