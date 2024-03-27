#pragma once

#include "constants.h"
#include "quaternion.h"

namespace hnn // https://github.com/hypernumbernet
{

class SplitOctonion
{
public:
    SplitOctonion() {}

    constexpr SplitOctonion(const double real, const double imaginary1, const double imaginary2, const double imaginary3,
                       const double imaginary4, const double imaginary5, const double imaginary6, const double imaginary7)
        : m_re(real), m_i1(imaginary1), m_i2(imaginary2), m_i3(imaginary3),
          m_i4(imaginary4), m_i5(imaginary5), m_i6(imaginary6), m_i7(imaginary7){}

    explicit SplitOctonion(const double a)
        : m_re(a), m_i1(0.0), m_i2(0.0), m_i3(0.0),
          m_i4(0.0), m_i5(0.0), m_i6(0.0), m_i7(0.0) {}

    double re() const { return m_re; }
    double i1() const { return m_i1; }
    double i2() const { return m_i2; }
    double i3() const { return m_i3; }
    double i4() const { return m_i4; }
    double i5() const { return m_i5; }
    double i6() const { return m_i6; }
    double i7() const { return m_i7; }

    void setRe(const double re) { m_re = re; }
    void setI1(const double i1) { m_i1 = i1; }
    void setI2(const double i2) { m_i2 = i2; }
    void setI3(const double i3) { m_i3 = i3; }
    void setI4(const double i4) { m_i4 = i4; }
    void setI5(const double i5) { m_i5 = i5; }
    void setI6(const double i6) { m_i6 = i6; }
    void setI7(const double i7) { m_i7 = i7; }

    void set(const double re)
    {
        m_re = re;
        m_i1 = 0.;
        m_i2 = 0.;
        m_i3 = 0.;
        m_i4 = 0.;
        m_i5 = 0.;
        m_i6 = 0.;
        m_i7 = 0.;
    }

    void set(const SplitOctonion& o)
    {
        m_re = o.re();
        m_i1 = o.i1();
        m_i2 = o.i2();
        m_i3 = o.i3();
        m_i4 = o.i4();
        m_i5 = o.i5();
        m_i6 = o.i6();
        m_i7 = o.i7();
    }

    const SplitOctonion operator+(const SplitOctonion& a) const
    {
        return SplitOctonion(m_re + a.m_re, m_i1 + a.m_i1, m_i2 + a.m_i2, m_i3 + a.m_i3,
                        m_i4 + a.m_i4, m_i5 + a.m_i5, m_i6 + a.m_i6, m_i7 + a.m_i7);
    }

    const SplitOctonion operator-(const SplitOctonion& a) const
    {
        return SplitOctonion(m_re - a.m_re, m_i1 - a.m_i1, m_i2 - a.m_i2, m_i3 - a.m_i3,
                        m_i4 - a.m_i4, m_i5 - a.m_i5, m_i6 - a.m_i6, m_i7 - a.m_i7);
    }

    const SplitOctonion operator+() const { return *this; }

    const SplitOctonion operator-() const {
        return SplitOctonion(-m_re, -m_i1, -m_i2, -m_i3, -m_i4, -m_i5, -m_i6, -m_i7);
    }

    SplitOctonion& operator+=(const SplitOctonion& a)
    {
        m_re += a.m_re;
        m_i1 += a.m_i1;
        m_i2 += a.m_i2;
        m_i3 += a.m_i3;
        m_i4 += a.m_i4;
        m_i5 += a.m_i5;
        m_i6 += a.m_i6;
        m_i7 += a.m_i7;
        return *this;
    }

    SplitOctonion& operator-=(const SplitOctonion& a)
    {
        m_re -= a.m_re;
        m_i1 -= a.m_i1;
        m_i2 -= a.m_i2;
        m_i3 -= a.m_i3;
        m_i4 -= a.m_i4;
        m_i5 -= a.m_i5;
        m_i6 -= a.m_i6;
        m_i7 -= a.m_i7;
        return *this;
    }

    template <typename E>
    SplitOctonion& operator*=(const E a)
    {
        m_re *= a;
        m_i1 *= a;
        m_i2 *= a;
        m_i3 *= a;
        m_i4 *= a;
        m_i5 *= a;
        m_i6 *= a;
        m_i7 *= a;
        return *this;
    }

    template <typename E>
    SplitOctonion& operator/=(const E a)
    {
        m_re /= a;
        m_i1 /= a;
        m_i2 /= a;
        m_i3 /= a;
        m_i4 /= a;
        m_i5 /= a;
        m_i6 /= a;
        m_i7 /= a;
        return *this;
    }

    const SplitOctonion operator*(const SplitOctonion& a) const
    {
        return SplitOctonion(
            m_re * a.m_re - m_i1 * a.m_i1 - m_i2 * a.m_i2 - m_i3 * a.m_i3 + m_i4 * a.m_i4 + m_i5 * a.m_i5 + m_i6 * a.m_i6 + m_i7 * a.m_i7,
            m_re * a.m_i1 + m_i1 * a.m_re + m_i2 * a.m_i3 - m_i3 * a.m_i2 + m_i4 * a.m_i5 - m_i5 * a.m_i4 + m_i6 * a.m_i7 - m_i7 * a.m_i6,
            m_re * a.m_i2 + m_i2 * a.m_re + m_i3 * a.m_i1 - m_i1 * a.m_i3 + m_i4 * a.m_i6 - m_i6 * a.m_i4 + m_i7 * a.m_i5 - m_i5 * a.m_i7,
            m_re * a.m_i3 + m_i3 * a.m_re + m_i1 * a.m_i2 - m_i2 * a.m_i1 + m_i4 * a.m_i7 - m_i7 * a.m_i4 + m_i5 * a.m_i6 - m_i6 * a.m_i5,
            m_re * a.m_i4 + m_i4 * a.m_re + m_i1 * a.m_i5 - m_i5 * a.m_i1 + m_i2 * a.m_i6 - m_i6 * a.m_i2 + m_i3 * a.m_i7 - m_i7 * a.m_i3,
            m_re * a.m_i5 + m_i5 * a.m_re + m_i3 * a.m_i6 - m_i6 * a.m_i3 + m_i4 * a.m_i1 - m_i1 * a.m_i4 + m_i7 * a.m_i2 - m_i2 * a.m_i7,
            m_re * a.m_i6 + m_i6 * a.m_re + m_i1 * a.m_i7 - m_i7 * a.m_i1 + m_i4 * a.m_i2 - m_i2 * a.m_i4 + m_i5 * a.m_i3 - m_i3 * a.m_i5,
            m_re * a.m_i7 + m_i7 * a.m_re + m_i2 * a.m_i5 - m_i5 * a.m_i2 + m_i4 * a.m_i3 - m_i3 * a.m_i4 + m_i6 * a.m_i1 - m_i1 * a.m_i6
        );
    }

    SplitOctonion cross(const SplitOctonion& a) const
    {
        return SplitOctonion(0,
            m_i2 * a.m_i3 - m_i3 * a.m_i2 + m_i4 * a.m_i5 - m_i5 * a.m_i4 + m_i6 * a.m_i7 - m_i7 * a.m_i6,
            m_i3 * a.m_i1 - m_i1 * a.m_i3 + m_i4 * a.m_i6 - m_i6 * a.m_i4 + m_i7 * a.m_i5 - m_i5 * a.m_i7,
            m_i1 * a.m_i2 - m_i2 * a.m_i1 + m_i4 * a.m_i7 - m_i7 * a.m_i4 + m_i5 * a.m_i6 - m_i6 * a.m_i5,
            m_i1 * a.m_i5 - m_i5 * a.m_i1 + m_i2 * a.m_i6 - m_i6 * a.m_i2 + m_i3 * a.m_i7 - m_i7 * a.m_i3,
            m_i3 * a.m_i6 - m_i6 * a.m_i3 + m_i4 * a.m_i1 - m_i1 * a.m_i4 + m_i7 * a.m_i2 - m_i2 * a.m_i7,
            m_i1 * a.m_i7 - m_i7 * a.m_i1 + m_i4 * a.m_i2 - m_i2 * a.m_i4 + m_i5 * a.m_i3 - m_i3 * a.m_i5,
            m_i2 * a.m_i5 - m_i5 * a.m_i2 + m_i4 * a.m_i3 - m_i3 * a.m_i4 + m_i6 * a.m_i1 - m_i1 * a.m_i6
        );
    }

    static SplitOctonion cross(const Quaternion& a, const Quaternion& b,
                                 const int w, const int x, const int y, const int z)
    {
        SplitOctonion o1(0);
        o1[w] = a.re(); o1[x] = a.i1(); o1[y] = a.i2(); o1[z] = a.i3();
        SplitOctonion o2(0);
        o2[w] = b.re(); o2[x] = b.i1(); o2[y] = b.i2(); o2[z] = b.i3();
        return o1.cross(o2);
    }

    template <typename E>
    const SplitOctonion operator*(const E a) const
    {
        return SplitOctonion(m_re * a, m_i1 * a, m_i2 * a, m_i3 * a, m_i4 * a, m_i5 * a, m_i6 * a, m_i7 * a);
    }

    template <typename E>
    friend const SplitOctonion operator*(const E a, const SplitOctonion& b)
    {
        return SplitOctonion(a * b.m_re, a * b.m_i1, a * b.m_i2, a * b.m_i3,
                        a * b.m_i4, a * b.m_i5, a * b.m_i6, a * b.m_i7);
    }

    const SplitOctonion operator/(const SplitOctonion& a) const
    {
        return (*this) * a.inversed();
    }

    bool operator==(const SplitOctonion& a) const
    {
        return m_re == a.m_re && m_i1 == a.m_i1 && m_i2 == a.m_i2 && m_i3 == a.m_i3 &&
               m_i4 == a.m_i4 && m_i5 == a.m_i5 && m_i6 == a.m_i6 && m_i7 == a.m_i7;
    }

    bool operator!=(const SplitOctonion& a) const
    {
        return m_re != a.m_i1 || m_i1 != a.m_i1 || m_i2 != a.m_i2 || m_i3 != a.m_i3 ||
               m_i4 != a.m_i4 || m_i5 != a.m_i5 || m_i6 != a.m_i6 || m_i7 != a.m_i7;
    }

    bool fuzzyCompare(const SplitOctonion& a) const
    {
        return hnn::fuzzyCompare(m_re, a.m_re) &&
                hnn::fuzzyCompare(m_i1, a.m_i1) &&
                hnn::fuzzyCompare(m_i2, a.m_i2) &&
                hnn::fuzzyCompare(m_i3, a.m_i3) &&
                hnn::fuzzyCompare(m_i4, a.m_i4) &&
                hnn::fuzzyCompare(m_i5, a.m_i5) &&
                hnn::fuzzyCompare(m_i6, a.m_i6) &&
                hnn::fuzzyCompare(m_i7, a.m_i7);
    }

    const double& operator[](const int i) const
    {
        if (i == 0) return m_re;
        if (i == 1) return m_i1;
        if (i == 2) return m_i2;
        if (i == 3) return m_i3;
        if (i == 4) return m_i4;
        if (i == 5) return m_i5;
        if (i == 6) return m_i6;
        return m_i7;
    }

    double& operator[](const int i)
    {
        if (i == 0) return m_re;
        if (i == 1) return m_i1;
        if (i == 2) return m_i2;
        if (i == 3) return m_i3;
        if (i == 4) return m_i4;
        if (i == 5) return m_i5;
        if (i == 6) return m_i6;
        return m_i7;
    }

    double norm() const
    {
        return m_re * m_re + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3
             - m_i4 * m_i4 - m_i5 * m_i5 - m_i6 * m_i6 - m_i7 * m_i7;
    }

    double abs() const
    {
        return sqrt(norm());
    }

    SplitOctonion& normalize()
    {
        return (*this) /= abs();
    }

    SplitOctonion conjugated() const
    {
        return SplitOctonion(m_re, -m_i1, -m_i2, -m_i3, -m_i4, -m_i5, -m_i6, -m_i7);
    }

    SplitOctonion inversed() const
    {
        double n = norm();
        return SplitOctonion(m_re / n, -m_i1 / n, -m_i2 / n, -m_i3 / n,
                      - m_i4 / n, -m_i5 / n, -m_i6 / n, -m_i7 / n);
    }

    static double angle(const double a0, const double a1, const double a2, const double a3,
                        const double b0, const double b1, const double b2, const double b3)
    {
        return acos(a0 * b0 + a1 * b1 + a2 * b2 + a3 * b3);
    }

    static double angle(const Quaternion& a, const Quaternion& b)
    {
        return acos(a.re() * b.re() + a.i1() * b.i1() + a.i2() * b.i2() + a.i3() * b.i3());
    }

    static Vector3 axis(const double a0, const double a1, const double a2, const double a3,
                        const double b0, const double b1, const double b2, const double b3)
    {
        const double v0 = a0 * b1 - a1 * b0 + a2 * b3 - a3 * b2;
        const double v1 = a0 * b2 - a2 * b0 + a3 * b1 - a1 * b3;
        const double v2 = a0 * b3 - a3 * b0 + a1 * b2 - a2 * b1;
        const double norm = v0 * v0 + v1 * v1 + v2 * v2;
        if (norm == 0.0)
            return Vector3(1.0, 0.0, 0.0);
        const double r = 1.0 / sqrt(norm);
        return Vector3(v0 * r, v1 * r, v2 * r);
    }

    static Vector3 axis(const Quaternion& a, const Quaternion& b)
    {
        return axis(a.re(), a.i1(), a.i2(), a.i3(), b.re(), b.i1(), b.i2(), b.i3());
    }

    static Quaternion rotor(const Vector3& axis, const double angle)
    {
        const double a = sin(-0.5 * angle);
        return Quaternion(cos(0.5 * angle), axis.x() * a, axis.y() * a, axis.z() * a);
    }

    static Quaternion rotation(const Quaternion& position, const Quaternion& rotor)
    {
        const double r0 = rotor.re();
        const double r1 = rotor.i1();
        const double r2 = rotor.i2();
        const double r3 = rotor.i3();
        const double x0 = position.re();
        const double x1 = position.i1();
        const double x2 = position.i2();
        const double x3 = position.i3();
        const double rr = r0 * r0 - r1 * r1 - r2 * r2 - r3 * r3;
        const double rd = 2.0 * r0;
        return Quaternion(rr * x0 - rd * (r1 * x1 + r2 * x2 + r3 * x3),
                          rr * x1 + rd * (r1 * x0 - r3 * x2 + r2 * x3),
                          rr * x2 + rd * (r2 * x0 + r3 * x1 - r1 * x3),
                          rr * x3 + rd * (r3 * x0 - r2 * x1 + r1 * x2));
    }

    bool fuzzyCompare(const SplitOctonion& a)
    {
        return hnn::fuzzyCompare(m_re, a.m_re) &&
               hnn::fuzzyCompare(m_i1, a.m_i1) &&
               hnn::fuzzyCompare(m_i2, a.m_i2) &&
               hnn::fuzzyCompare(m_i3, a.m_i3) &&
               hnn::fuzzyCompare(m_i4, a.m_i4) &&
               hnn::fuzzyCompare(m_i5, a.m_i5) &&
               hnn::fuzzyCompare(m_i6, a.m_i6) &&
               hnn::fuzzyCompare(m_i7, a.m_i7);
    }

    friend std::ostream& operator<<(std::ostream& os, const SplitOctonion& a)
    {
        os << a.m_re << ", " << a.m_i1 << ", " << a.m_i2 << ", " << a.m_i3 << ", " <<
              a.m_i4 << ", " << a.m_i5 << ", " << a.m_i6 << ", " << a.m_i7;
        return os;
    }

private:
    double m_re;
    double m_i1;
    double m_i2;
    double m_i3;
    double m_i4;
    double m_i5;
    double m_i6;
    double m_i7;
};

} // namespace
