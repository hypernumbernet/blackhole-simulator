#pragma once

#include "constants.h"
#include <cmath>
#include <string>
#include <sstream>

namespace hnn // https://github.com/hypernumbernet
{

class Bioctonion
{
public:
    Bioctonion() {}

    constexpr Bioctonion(
            const double a, const double b, const double c, const double d,
            const double p, const double q, const double r, const double s)
        : m_a(a), m_b(b), m_c(c), m_d(d),
          m_p(p), m_q(q), m_r(r), m_s(s) {}

    explicit Bioctonion(const double a)
        : m_a(a), m_b(0.0), m_c(0.0), m_d(0.0),
          m_p(0.0), m_q(0.0), m_r(0.0), m_s(0.0) {}

    double a() const { return m_a; }
    double b() const { return m_b; }
    double c() const { return m_c; }
    double d() const { return m_d; }
    double p() const { return m_p; }
    double q() const { return m_q; }
    double r() const { return m_r; }
    double s() const { return m_s; }

    void setA(const double a) { m_a = a; }
    void setB(const double b) { m_b = b; }
    void setC(const double c) { m_c = c; }
    void setD(const double d) { m_d = d; }
    void setP(const double p) { m_p = p; }
    void setQ(const double q) { m_q = q; }
    void setR(const double r) { m_r = r; }
    void setS(const double s) { m_s = s; }

    const Bioctonion operator+(const Bioctonion& a) const
    {
        return Bioctonion(m_a + a.m_a, m_b + a.m_b, m_c + a.m_c, m_d + a.m_d,
                          m_p + a.m_p, m_q + a.m_q, m_r + a.m_r, m_s + a.m_s);
    }

    const Bioctonion operator-(const Bioctonion& a) const
    {
        return Bioctonion(m_a - a.m_a, m_b - a.m_b, m_c - a.m_c, m_d - a.m_d,
                          m_p - a.m_p, m_q - a.m_q, m_r - a.m_r, m_s - a.m_s);
    }

    const Bioctonion operator+() const { return *this; }

    const Bioctonion operator-() const {
        return Bioctonion(-m_a, -m_b, -m_c, -m_d, -m_p, -m_q, -m_r, -m_s);
    }

    Bioctonion& operator+=(const Bioctonion& a)
    {
        m_a += a.m_a;
        m_b += a.m_b;
        m_c += a.m_c;
        m_d += a.m_d;
        m_p += a.m_p;
        m_q += a.m_q;
        m_r += a.m_r;
        m_s += a.m_s;
        return *this;
    }

    Bioctonion& operator-=(const Bioctonion& a)
    {
        m_a -= a.m_a;
        m_b -= a.m_b;
        m_c -= a.m_c;
        m_d -= a.m_d;
        m_p -= a.m_p;
        m_q -= a.m_q;
        m_r -= a.m_r;
        m_s -= a.m_s;
        return *this;
    }

    template <typename E>
    Bioctonion& operator*=(E a)
    {
        m_a *= a;
        m_b *= a;
        m_c *= a;
        m_d *= a;
        m_p *= a;
        m_q *= a;
        m_r *= a;
        m_s *= a;
        return *this;
    }

    template <typename E>
    Bioctonion& operator/=(E a)
    {
        m_a /= a;
        m_b /= a;
        m_c /= a;
        m_d /= a;
        m_p /= a;
        m_q /= a;
        m_r /= a;
        m_s /= a;
        return *this;
    }

    /*
    unit definition: h, i, j, k

    hh = ii = jj = kk = -1
    ij = -ji = k, jk = -kj = i, ki = -ik = j
    hi = ih, hj = jh, hk = kh

    octonion o1 = a1 h + b1 i + c1 j + d1 k + p1 + q1 hi + r1 hj + s1 hk
    octonion o2 = a2 h + b2 i + c2 j + d2 k + p2 + q2 hi + r2 hj + s2 hk

    (a1 h + b1 i + c1 j + d1 k + p1 + q1 hi + r1 hj + s1 hk)(a2 h + b2 i + c2 j + d2 k + p2 + q2 hi + r2 hj + s2 hk)

    =   a1a2 hh  + a1b2 hi  + a1c2 hj  + a1d2 hk  + a1p2 h  + a1q2 hhi  + a1r2 hhj  + a1s2 hhk
      + b1a2 ih  + b1b2 ii  + b1c2 ij  + b1d2 ik  + b1p2 i  + b1q2 ihi  + b1r2 ihj  + b1s2 ihk
      + c1a2 jh  + c1b2 ji  + c1c2 jj  + c1d2 jk  + c1p2 j  + c1q2 jhi  + c1r2 jhj  + c1s2 jhk
      + d1a2 kh  + d1b2 ki  + d1c2 kj  + d1d2 kk  + d1p2 k  + d1q2 khi  + d1r2 khj  + d1s2 khk
      + p1a2 h   + p1b2 i   + p1c2 j   + p1d2 k   + p1p2    + p1q2 hi   + p1r2 hj   + p1s2 hk
      + q1a2 hih + q1b2 hii + q1c2 hij + q1d2 hik + q1p2 hi + q1q2 hihi + q1r2 hihj + q1s2 hihk
      + r1a2 hjh + r1b2 hji + r1c2 hjj + r1d2 hjk + r1p2 hj + r1q2 hjhi + r1r2 hjhj + r1s2 hjhk
      + s1a2 hkh + s1b2 hki + s1c2 hkj + s1d2 hkk + s1p2 hk + s1q2 hkhi + s1r2 hkhj + s1s2 hkhk

    = - a1a2    + a1b2 hi + a1c2 hj + a1d2 hk + a1p2 h  - a1q2 i  - a1r2 j  - a1s2 k
      + b1a2 hi - b1b2    + b1c2 k  - b1d2 j  + b1p2 i  - b1q2 h  + b1r2 hk - b1s2 hj
      + c1a2 hj - c1b2 k  - c1c2    + c1d2 i  + c1p2 j  - c1q2 hk - c1r2 h  + c1s2 hi
      + d1a2 hk + d1b2 j  - d1c2 i  - d1d2    + d1p2 k  + d1q2 hj - d1r2 hi - d1s2 h
      + p1a2 h  + p1b2 i  + p1c2 j  + p1d2 k  + p1p2    + p1q2 hi + p1r2 hj + p1s2 hk
      - q1a2 i  - q1b2 h  + q1c2 hk - q1d2 hj + q1p2 hi + q1q2    - q1r2 k  + q1s2 j
      - r1a2 j  - r1b2 hk - r1c2 h  + r1d2 hi + r1p2 hj + r1q2 k  + r1r2    - r1s2 i
      - s1a2 k  + s1b2 hj - s1c2 hi - s1d2 h  + s1p2 hk - s1q2 j  + s1r2 i  + s1s2

    =   (  a1p2 - b1q2 - c1r2 - d1s2 + p1a2 - q1b2 - r1c2 - s1d2) h
      + (- a1q2 + b1p2 + c1d2 - d1c2 + p1b2 - q1a2 - r1s2 + s1r2) i
      + (- a1r2 - b1d2 + c1p2 + d1b2 + p1c2 + q1s2 - r1a2 - s1q2) j
      + (- a1s2 + b1c2 - c1b2 + d1p2 + p1d2 - q1r2 + r1q2 - s1a2) k
         - a1a2 - b1b2 - c1c2 - d1d2 + p1p2 + q1q2 + r1r2 + s1s2
      + (  a1b2 + b1a2 + c1s2 - d1r2 + p1q2 + q1p2 + r1d2 - s1c2) hi
      + (  a1c2 - b1s2 + c1a2 + d1q2 + p1r2 - q1d2 + r1p2 + s1b2) hj
      + (  a1d2 + b1r2 - c1q2 + d1a2 + p1s2 + q1c2 - r1b2 + s1p2) hk
    */
    const Bioctonion operator*(const Bioctonion& o) const
    {
        return Bioctonion(
            m_a * o.m_p - m_b * o.m_q - m_c * o.m_r - m_d * o.m_s + m_p * o.m_a - m_q * o.m_b - m_r * o.m_c - m_s * o.m_d,
          - m_a * o.m_q + m_b * o.m_p + m_c * o.m_d - m_d * o.m_c + m_p * o.m_b - m_q * o.m_a - m_r * o.m_s + m_s * o.m_r,
          - m_a * o.m_r - m_b * o.m_d + m_c * o.m_p + m_d * o.m_b + m_p * o.m_c + m_q * o.m_s - m_r * o.m_a - m_s * o.m_q,
          - m_a * o.m_s + m_b * o.m_c - m_c * o.m_b + m_d * o.m_p + m_p * o.m_d - m_q * o.m_r + m_r * o.m_q - m_s * o.m_a,
          - m_a * o.m_a - m_b * o.m_b - m_c * o.m_c - m_d * o.m_d + m_p * o.m_p + m_q * o.m_q + m_r * o.m_r + m_s * o.m_s,
          + m_a * o.m_b + m_b * o.m_a + m_c * o.m_s - m_d * o.m_r + m_p * o.m_q + m_q * o.m_p + m_r * o.m_d - m_s * o.m_c,
          + m_a * o.m_c - m_b * o.m_s + m_c * o.m_a + m_d * o.m_q + m_p * o.m_r - m_q * o.m_d + m_r * o.m_p + m_s * o.m_b,
          + m_a * o.m_d + m_b * o.m_r - m_c * o.m_q + m_d * o.m_a + m_p * o.m_s + m_q * o.m_c - m_r * o.m_b + m_s * o.m_p
        );
    }

    template <typename E>
    const Bioctonion operator*(const E x) const
    {
        return Bioctonion(m_a * x, m_b * x, m_c * x, m_d * x, m_p * x, m_q * x, m_r * x, m_s * x);
    }

    template <typename E>
    friend const Bioctonion operator*(const E x, const Bioctonion& o)
    {
        return Bioctonion(x * o.m_a, x * o.m_b, x * o.m_c, x * o.m_d,
                          x * o.m_p, x * o.m_q, x * o.m_r, x * o.m_s);
    }

    template <typename E>
    const Bioctonion operator/(const E x) const
    {
        return Bioctonion(m_a / x, m_b / x, m_c / x, m_d / x, m_p / x, m_q / x, m_r / x, m_s / x);
    }

    const Bioctonion operator/(const Bioctonion& o) const
    {
        return ((*this) * o.conjugated()) / o.norm();
    }

    bool operator==(const Bioctonion& a) const
    {
        return m_a == a.m_a && m_b == a.m_b && m_c == a.m_c && m_d == a.m_d &&
               m_p == a.m_p && m_q == a.m_q && m_r == a.m_r && m_s == a.m_s;
    }

    bool operator!=(const Bioctonion& a) const
    {
        return m_a != a.m_b || m_b != a.m_b || m_c != a.m_c || m_d != a.m_d ||
               m_p != a.m_p || m_q != a.m_q || m_r != a.m_r || m_s != a.m_s;
    }

    bool fuzzyCompare(const Bioctonion& a) const
    {
        return hnn::fuzzyCompare(m_a, a.m_a) &&
               hnn::fuzzyCompare(m_b, a.m_b) &&
               hnn::fuzzyCompare(m_c, a.m_c) &&
               hnn::fuzzyCompare(m_d, a.m_d) &&
               hnn::fuzzyCompare(m_p, a.m_p) &&
               hnn::fuzzyCompare(m_q, a.m_q) &&
               hnn::fuzzyCompare(m_r, a.m_r) &&
               hnn::fuzzyCompare(m_s, a.m_s);
    }

    const double& operator[](int index) const
    {
        return array[index];
    }

    double& operator[](int index)
    {
        return array[index];
    }

    /*
    (a h + b i + c j + d k + p + q hi + r hj + s hk)(a h - b i - c j - d k + p - q hi - r hj - s hk)
    =   aa hh  - ab hi  - ac hj  - ad hk  + ap h  - aq hhi  - ar hhj  - as hhk
      + ba ih  - bb ii  - bc ij  - bd ik  + bp i  - bq ihi  - br ihj  - bs ihk
      + ca jh  - cb ji  - cc jj  - cd jk  + cp j  - cq jhi  - cr jhj  - cs jhk
      + da kh  - db ki  - dc kj  - dd kk  + dp k  - dq khi  - dr khj  - ds khk
      + pa h   - pb i   - pc j   - pd k   + pp    - pq hi   - pr hj   - ps hk
      + qa hih - qb hii - qc hij - qd hik + qp hi - qq hihi - qr hihj - qs hihk
      + ra hjh - rb hji - rc hjj - rd hjk + rp hj - rq hjhi - rr hjhj - rs hjhk
      + sa hkh - sb hki - sc hkj - sd hkk + sp hk - sq hkhi - sr hkhj - ss hkhk
    = - aa     - ab hi  - ac hj  - ad hk  + ap h  + aq i    + ar j    + as k
      + ba hi  + bb     - bc k   + bd j   + bp i  + bq h    - br hk   + bs hj
      + ca hj  + cb k   + cc     - cd i   + cp j  + cq hk   + cr h    - cs hi
      + da hk  - db j   + dc i   + dd     + dp k  - dq hj   + dr hi   + ds h
      + pa h   - pb i   - pc j   - pd k   + pp    - pq hi   - pr hj   - ps hk
      - qa i   + qb h   - qc hk  + qd hj  + qp hi - qq      + qr k    - qs j
      - ra j   + rb hk  + rc h   - rd hi  + rp hj - rq k    - rr      + rs i
      - sa k   - sb hj  + sc hi  + sd h   + sp hk + sq j    - sr i    - ss
    = 2 (ap + bq + cr + ds) h - aa + bb + cc + dd + pp - qq - rr - ss
    */
    double norm() const
    {
        double a = 2.0 * (m_a * m_p + m_b * m_q + m_c * m_r + m_d * m_s);
        double p = - m_a * m_a + m_b * m_b + m_c * m_c + m_d * m_d
                   + m_p * m_p - m_q * m_q - m_r * m_r - m_s * m_s;
        return a * a + p * p;
    }

    double abs() const
    {
        return sqrt(norm());
    }

    Bioctonion& normalize()
    {
        return (*this) /= abs();
    }

    Bioctonion conjugated() const
    {
        return Bioctonion(m_a, - m_b, - m_c, - m_d, m_p, - m_q, - m_r, - m_s);
    }

    Bioctonion inversed() const
    {
        double n = norm();
        return Bioctonion(m_a / n, -m_b / n, -m_c / n, -m_d / n,
                          m_p / n, -m_q / n, -m_r / n, -m_s / n);
    }

    std::string toString()
    {
        std::ostringstream o;
        o << m_a << ", " << m_b << ", " << m_c << ", " << m_d << ", " <<
             m_p << ", " << m_q << ", " << m_r << ", " << m_s;
        return o.str();
    }

private:
    union
        {
            struct
            {
                double m_a;
                double m_b;
                double m_c;
                double m_d;
                double m_p;
                double m_q;
                double m_r;
                double m_s;
            };
            double array[8];
        };
};

} // namespace
