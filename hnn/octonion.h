#pragma once

#include "quaternion.h"

namespace hnn // https://github.com/hypernumbernet
{

template <typename T>
class Octonion
{
public:
    Octonion() {}

    constexpr Octonion(const T real, const T imaginary1, const T imaginary2, const T imaginary3,
                       const T imaginary4, const T imaginary5, const T imaginary6, const T imaginary7)
        : m_re(real), m_i1(imaginary1), m_i2(imaginary2), m_i3(imaginary3),
          m_i4(imaginary4), m_i5(imaginary5), m_i6(imaginary6), m_i7(imaginary7){}

    constexpr explicit Octonion(const T a)
        : m_re(T(a)), m_i1(T(0)), m_i2(T(0)), m_i3(T(0)),
          m_i4(T(0)), m_i5(T(0)), m_i6(T(0)), m_i7(T(0)) {}

    constexpr T re() const { return m_re; }
    constexpr T i1() const { return m_i1; }
    constexpr T i2() const { return m_i2; }
    constexpr T i3() const { return m_i3; }
    constexpr T i4() const { return m_i4; }
    constexpr T i5() const { return m_i5; }
    constexpr T i6() const { return m_i6; }
    constexpr T i7() const { return m_i7; }

    void setRe(T re) { m_re = re; }
    void setI1(T i1) { m_i1 = i1; }
    void setI2(T i2) { m_i2 = i2; }
    void setI3(T i3) { m_i3 = i3; }
    void setI4(T i4) { m_i4 = i4; }
    void setI5(T i5) { m_i5 = i5; }
    void setI6(T i6) { m_i6 = i6; }
    void setI7(T i7) { m_i7 = i7; }

    constexpr const Octonion operator+(const Octonion& a) const
    {
        return Octonion(m_re + a.m_re, m_i1 + a.m_i1, m_i2 + a.m_i2, m_i3 + a.m_i3,
                        m_i4 + a.m_i4, m_i5 + a.m_i5, m_i6 + a.m_i6, m_i7 + a.m_i7);
    }

    constexpr const Octonion operator-(const Octonion& a) const
    {
        return Octonion(m_re - a.m_re, m_i1 - a.m_i1, m_i2 - a.m_i2, m_i3 - a.m_i3,
                        m_i4 - a.m_i4, m_i5 - a.m_i5, m_i6 - a.m_i6, m_i7 - a.m_i7);
    }

    constexpr const Octonion operator+() const { return *this; }

    constexpr const Octonion operator-() const {
        return Octonion(-m_re, -m_i1, -m_i2, -m_i3, -m_i4, -m_i5, -m_i6, -m_i7);
    }

    Octonion& operator+=(const Octonion& a)
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

    Octonion& operator-=(const Octonion& a)
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
    Octonion& operator*=(E a)
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
    Octonion& operator/=(E a)
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

    /**
    octonion unit definition: i, j, k, l, m, n, o

    ij = k, jk = i, ki = j
    il = m, lm = i, mi = l
    jl = n, ln = j, nj = l
    kl = o, lo = k, ok = l
    mk = n, kn = m, nm = k
    ni = o, io = n, on = i
    oj = m, jm = o, mo = j

    octonion a = a0 + a1i + a2j + a3k + a4l + a5m + a6n + a7o
    octonion b = b0 + b1i + b2j + b3k + b4l + b5m + b6n + b7o

    (a1i + a2j + a3k + a4l + a5m + a6n + a7o)(b1i + b2j + b3k + b4l + b5m + b6n + b7o)
    =   a1b1ii + a1b2ij + a1b3ik + a1b4il + a1b5im + a1b6in + a1b7io
      + a2b1ji + a2b2jj + a2b3jk + a2b4jl + a2b5jm + a2b6jn + a2b7jo
      + a3b1ki + a3b2kj + a3b3kk + a3b4kl + a3b5km + a3b6kn + a3b7ko
      + a4b1li + a4b2lj + a4b3lk + a4b4ll + a4b5lm + a4b6ln + a4b7lo
      + a5b1mi + a5b2mj + a5b3mk + a5b4ml + a5b5mm + a5b6mn + a5b7mo
      + a6b1ni + a6b2nj + a6b3nk + a6b4nl + a6b5nm + a6b6nn + a6b7no
      + a7b1oi + a7b2oj + a7b3ok + a7b4ol + a7b5om + a7b6on + a7b7oo
    = - a1b1 + a1b2k - a1b3j + a1b4m - a1b5l - a1b6o + a1b7n
      - a2b1k - a2b2 + a2b3i + a2b4n + a2b5o - a2b6l - a2b7m
      + a3b1j - a3b2i - a3b3 + a3b4o - a3b5n + a3b6m - a3b7l
      - a4b1m - a4b2n - a4b3o - a4b4 + a4b5i + a4b6j + a4b7k
      + a5b1l - a5b2o + a5b3n - a5b4i - a5b5 - a5b6k + a5b7j
      + a6b1o + a6b2l - a6b3m - a6b4j + a6b5k - a6b6 - a6b7i
      - a7b1n + a7b2m + a7b3l - a7b4k - a7b5j + a7b6i - a7b7
    =  − a1​b1​ - a2​b2 - a3​b3​ - a4​b4​ - a5​b5 ​- a6​b6 - a7​b7
      + (  a2​b3​ − a3​b2​ + a4​b5​ − a5​b4​ − a6​b7​ + a7​b6​)i
      + (​− a1​b3 + a3​b1​ + a4​b6 ​+ a5​b7​ − a6​b4 ​− a7​b5​)j
      + (  a1​b2​ − a2​b1​ + a4​b7 - a5​b6 + a6​b5 - a7​b4​​)k
      + (- a1​b5 - a2​b6 - a3​b7 + a5​b1​ + a6​b2​ + a7​b3​)l
      + (  a1​b4 - a2​b7​ + a3​b6 - a4​b1​ - a6​b3 + a7​b2​)m
      + (  a1​b7​ + a2​b4 - a3​b5 - a4​b2 + a5​b3​ - a7​b1​)n
      + (- a1​b6 + a2​b5​ + a3​b4 - a4​b3 - a5​b2 + a6​b1​​)o​
    */
    constexpr const Octonion operator*(const Octonion& a) const
    {
        return Octonion(
            m_re * a.m_re - m_i1 * a.m_i1 - m_i2 * a.m_i2 - m_i3 * a.m_i3 - m_i4 * a.m_i4 - m_i5 * a.m_i5 - m_i6 * a.m_i6 - m_i7 * a.m_i7,
            m_re * a.m_i1 + m_i1 * a.m_re + m_i2 * a.m_i3 - m_i3 * a.m_i2 + m_i4 * a.m_i5 - m_i5 * a.m_i4 - m_i6 * a.m_i7 + m_i7 * a.m_i6,
            m_re * a.m_i2 + m_i2 * a.m_re - m_i1 * a.m_i3 + m_i3 * a.m_i1 + m_i4 * a.m_i6 + m_i5 * a.m_i7 - m_i6 * a.m_i4 - m_i7 * a.m_i5,
            m_re * a.m_i3 + m_i3 * a.m_re + m_i1 * a.m_i2 - m_i2 * a.m_i1 + m_i4 * a.m_i7 - m_i5 * a.m_i6 + m_i6 * a.m_i5 - m_i7 * a.m_i4,
            m_re * a.m_i4 + m_i4 * a.m_re - m_i1 * a.m_i5 - m_i2 * a.m_i6 - m_i3 * a.m_i7 + m_i5 * a.m_i1 + m_i6 * a.m_i2 + m_i7 * a.m_i3,
            m_re * a.m_i5 + m_i5 * a.m_re + m_i1 * a.m_i4 - m_i2 * a.m_i7 + m_i3 * a.m_i6 - m_i4 * a.m_i1 - m_i6 * a.m_i3 + m_i7 * a.m_i2,
            m_re * a.m_i6 + m_i6 * a.m_re + m_i1 * a.m_i7 + m_i2 * a.m_i4 - m_i3 * a.m_i5 - m_i4 * a.m_i2 + m_i5 * a.m_i3 - m_i7 * a.m_i1,
            m_re * a.m_i7 + m_i7 * a.m_re - m_i1 * a.m_i6 + m_i2 * a.m_i5 + m_i3 * a.m_i4 - m_i4 * a.m_i3 - m_i5 * a.m_i2 + m_i6 * a.m_i1
        );
    }

    template <typename E>
    constexpr const Octonion operator*(E a) const
    {
        return Octonion(m_re * a, m_i1 * a, m_i2 * a, m_i3 * a, m_i4 * a, m_i5 * a, m_i6 * a, m_i7 * a);
    }

    template <typename E>
    constexpr friend inline const Octonion operator*(E a, const Octonion& b)
    {
        return Octonion(a * b.m_re, a * b.m_i1, a * b.m_i2, a * b.m_i3,
                        a * b.m_i4, a * b.m_i5, a * b.m_i6, a * b.m_i7);
    }

    constexpr const Octonion operator/(const Octonion& a) const
    {
        return (*this) * a.inversed();
    }

    constexpr bool operator==(const Octonion& a) const
    {
        return m_re == a.m_re && m_i1 == a.m_i1 && m_i2 == a.m_i2 && m_i3 == a.m_i3 &&
                m_i4 == a.m_i4 && m_i5 == a.m_i5 && m_i6 == a.m_i6 && m_i7 == a.m_i7;
    }

    constexpr bool operator!=(const Octonion& a) const
    {
        return m_re != a.m_i1 || m_i1 != a.m_i1 || m_i2 != a.m_i2 || m_i3 != a.m_i3 ||
                m_i4 != a.m_i4 || m_i5 != a.m_i5 || m_i6 != a.m_i6 || m_i7 != a.m_i7;
    }

    constexpr bool fuzzyCompare(const Octonion& a) const
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

    constexpr T norm() const
    {
        return m_re * m_re + m_i1 * m_i1 + m_i2 * m_i2 + m_i3 * m_i3 +
                m_i4 * m_i4 + m_i5 * m_i5 + m_i6 * m_i6 + m_i7 * m_i7;
    }

    constexpr T abs() const
    {
        return sqrt(norm());
    }

    Octonion& normalize()
    {
        return (*this) /= abs();
    }

    constexpr Octonion conjugated() const
    {
        return Octonion(m_re, -m_i1, -m_i2, -m_i3, -m_i4, -m_i5, -m_i6, -m_i7);
    }

    constexpr Octonion inversed() const
    {
        T n = norm();
        return Octonion(m_re / n, -m_i1 / n, -m_i2 / n, -m_i3 / n,
                        -m_i4 / n, -m_i5 / n, -m_i6 / n, -m_i7 / n);
    }

    std::string toString()
    {
        return std::to_string(m_re) + ", " + std::to_string(m_i1) +
                ", " + std::to_string(m_i2) + ", " + std::to_string(m_i3) +
                ", " + std::to_string(m_i4) + ", " + std::to_string(m_i5) +
                ", " + std::to_string(m_i6) + ", " + std::to_string(m_i7);
    }

private:
    T m_re, m_i1, m_i2, m_i3, m_i4, m_i5, m_i6, m_i7;
};

} // namespace
