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
          m_i4(imaginary4), m_i5(imaginary5), m_i6(imaginary6), m_i7(imaginary7) {}

    explicit SplitOctonion(const double a)
        : m_re(a), m_i1(0.), m_i2(0.), m_i3(0.), m_i4(0.), m_i5(0.), m_i6(0.), m_i7(0.) {}

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
        return SplitOctonion(0.,
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
        SplitOctonion o1(0.);
        o1[w] = a.re(); o1[x] = a.i1(); o1[y] = a.i2(); o1[z] = a.i3();
        SplitOctonion o2(0.);
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
        double l = sqrt(m_re * m_re + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3);
        double r = sqrt(m_i4 * m_i4 + m_i5 * m_i5 + m_i6 * m_i6 + m_i7 * m_i7);
        if (l == 0.) {
            m_re = 1.; m_i1 = 0.; m_i2 = 0.; m_i3 = 0.;
        } else {
            m_re /= l; m_i1 /= l; m_i2 /= l; m_i3 /= l;
        }
        if (r == 0.) {
            m_i4 = 1.; m_i5 = 0.; m_i6 = 0.; m_i7 = 0.;
        } else {
            m_i4 /= r; m_i5 /= r; m_i6 /= r; m_i7 /= r;
        }
        return *this;
    }

    SplitOctonion conjugated() const
    {
        return SplitOctonion(m_re, -m_i1, -m_i2, -m_i3, -m_i4, -m_i5, -m_i6, -m_i7);
    }

    SplitOctonion inversed() const
    {
        double n = norm();
        if (n == 0.)
            return conjugated();
        return SplitOctonion(m_re / n, -m_i1 / n, -m_i2 / n, -m_i3 / n, -m_i4 / n, -m_i5 / n, -m_i6 / n, -m_i7 / n);
    }

    // for normalized split-octonion
    std::pair<double, double> angle(SplitOctonion x)
    {
        return {acos(m_i1 * x.i1() + m_i2 * x.i2() + m_i3 * x.i3()),
                acos(m_i4 * x.i4() + m_i5 * x.i5() + m_i6 * x.i6() + m_i7 * x.i7())};
    }

    // for normalized split-octonion
    static SplitOctonion axis(const SplitOctonion& u, const SplitOctonion& v)
    {
        SplitOctonion w = u.cross(v);
        return w.normalize();
    }

    static SplitOctonion rotor(const SplitOctonion& axis, const double alpha, const double beta)
    {
        double a = -0.5 * alpha;
        double b = -0.5 * beta;
        double sa = sin(a);
        double sb = sinh(b);
        SplitOctonion el(cos(a), axis.i1() * sa, axis.i2() * sa, axis.i3() * sa, 0., 0., 0., 0.);
        SplitOctonion er(cosh(b), 0., 0., 0., axis.i4() * sb, axis.i5() * sb, axis.i6() * sb, axis.i7() * sb);
        return el * er;
    }

    SplitOctonion rotation(const SplitOctonion& position)
    {
        return (*this).conjugated() * position * (*this);
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
