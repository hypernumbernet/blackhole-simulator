#pragma once

#include "vector3.h"
#include <complex>

namespace hnn // https://github.com/hypernumbernet
{

using complex = std::complex<double>;

class Biquaternion
{
public:
    constexpr Biquaternion() {}

    constexpr Biquaternion(const complex& w, const complex& x, const complex& y, const complex& z)
        : m_w(w), m_x(x), m_y(y), m_z(z) {}

    explicit constexpr Biquaternion(const complex& w)
        : m_w(w), m_x(), m_y(), m_z() {}

    explicit constexpr Biquaternion(const Vector3& v)
        : m_w(), m_x(0.0, v.x()), m_y(0.0, v.y()), m_z(0.0, v.z()) {}

    template <typename E>
    constexpr Biquaternion(const double* const a, const E index)
        : m_w(a[index], 0.0), m_x(0.0, a[index + 1]), m_y(0.0, a[index + 2]), m_z(0.0, a[index + 3]) {}

    constexpr Biquaternion(const double w, const double x, const double y, const double z)
        : m_w(w, 0.0), m_x(0.0, x), m_y(0.0, y), m_z(0.0, z) {}

    constexpr Biquaternion(const double real, const double h, const double i, const double hi,
                           const double j, const double hj, const double k, const double hk)
        : m_w(real, h), m_x(i, hi), m_y(j, hj), m_z(k, hk) {}

    complex w() const { return m_w; }
    complex x() const { return m_x; }
    complex y() const { return m_y; }
    complex z() const { return m_z; }

    void setW(const complex& w) { m_w = w; }
    void setX(const complex& x) { m_x = x; }
    void setY(const complex& y) { m_y = y; }
    void setZ(const complex& z) { m_z = z; }

    double re() const { return m_w.real(); }
    double h()  const { return m_w.imag(); }
    double i()  const { return m_x.real(); }
    double hi() const { return m_x.imag(); }
    double j()  const { return m_y.real(); }
    double hj() const { return m_y.imag(); }
    double k()  const { return m_z.real(); }
    double hk() const { return m_z.imag(); }

    void setRe(const double a) { m_w.real(a); }
    void setH (const double a) { m_w.imag(a); }
    void setI (const double a) { m_x.real(a); }
    void setHi(const double a) { m_x.imag(a); }
    void setJ (const double a) { m_y.real(a); }
    void setHj(const double a) { m_y.imag(a); }
    void setK (const double a) { m_z.real(a); }
    void setHk(const double a) { m_z.imag(a); }

    void set(const complex& w, const complex& x, const complex& y, const complex& z)
    {
        m_w = w;
        m_x = x;
        m_y = y;
        m_z = z;
    }

    void set(const double* const a)
    {
        m_w.real(a[0]);
        m_x.imag(a[1]);
        m_y.imag(a[2]);
        m_z.imag(a[3]);
    }

    template <typename E>
    void set(const double* const a, const E index)
    {
        m_w.real(a[index]);
        m_x.imag(a[index + 1]);
        m_y.imag(a[index + 2]);
        m_z.imag(a[index + 3]);
    }

    static constexpr Biquaternion zero()
    {
        return Biquaternion();
    }

    static constexpr Biquaternion identity()
    {
        return Biquaternion(1.0, 0.0, 0.0, 0.0);
    }

    const Biquaternion operator+(const Biquaternion& a) const
    {
        return Biquaternion(m_w + a.m_w, m_x + a.m_x, m_y + a.m_y, m_z + a.m_z);
    }

    const Biquaternion operator-(const Biquaternion& a) const
    {
        return Biquaternion(m_w - a.m_w, m_x - a.m_x, m_y - a.m_y, m_z - a.m_z);
    }

    const Biquaternion operator+() const { return *this; }

    const Biquaternion operator-() const { return Biquaternion(-m_w, -m_x, -m_y, -m_z); }

    Biquaternion& operator+=(const Biquaternion& a)
    {
        m_w += a.m_w;
        m_x += a.m_x;
        m_y += a.m_y;
        m_z += a.m_z;
        return *this;
    }

    Biquaternion& operator-=(const Biquaternion& a)
    {
        m_w -= a.m_w;
        m_x -= a.m_x;
        m_y -= a.m_y;
        m_z -= a.m_z;
        return *this;
    }

    const Biquaternion operator*(const Biquaternion& a) const
    {
        return Biquaternion(
            m_w * a.m_w - m_x * a.m_x - m_y * a.m_y - m_z * a.m_z,
            m_w * a.m_x + m_x * a.m_w + m_y * a.m_z - m_z * a.m_y,
            m_w * a.m_y - m_x * a.m_z + m_y * a.m_w + m_z * a.m_x,
            m_w * a.m_z + m_x * a.m_y - m_y * a.m_x + m_z * a.m_w
        );
    }

    template <typename E>
    const Biquaternion operator*(const E a) const
    {
        return Biquaternion(m_w * a, m_x * a, m_y * a, m_z * a);
    }

    template <typename E>
    friend const Biquaternion operator*(const E a, const Biquaternion& b)
    {
        return Biquaternion(a * b.m_w, a * b.m_x, a * b.m_y, a * b.m_z);
    }

    template <typename E>
    const Biquaternion operator/(const E a) const
    {
        return Biquaternion(m_w / a, m_x / a, m_y / a, m_z / a);
    }

    Biquaternion& operator*=(const Biquaternion& a)
    {
        set(
            m_w * a.m_w - m_x * a.m_x - m_y * a.m_y - m_z * a.m_z,
            m_w * a.m_x + m_x * a.m_w + m_y * a.m_z - m_z * a.m_y,
            m_w * a.m_y - m_x * a.m_z + m_y * a.m_w + m_z * a.m_x,
            m_w * a.m_z + m_x * a.m_y - m_y * a.m_x + m_z * a.m_w
        );
        return *this;
    }

    template <typename E>
    Biquaternion& operator*=(E a)
    {
        m_w *= a;
        m_x *= a;
        m_y *= a;
        m_z *= a;
        return *this;
    }

    bool operator==(const Biquaternion& a) const
    {
        return m_w == a.m_w && m_x == a.m_x && m_y == a.m_y && m_z == a.m_z;
    }

    bool operator!=(const Biquaternion& a) const
    {
        return m_w != a.m_x || m_x != a.m_x || m_y != a.m_y || m_z != a.m_z;
    }

    Biquaternion biconjugated() const
    {
        return Biquaternion(m_w, - m_x, - m_y, - m_z);
    }

    Biquaternion complexConjugated() const
    {
        return Biquaternion(std::conj(m_w), std::conj(m_x), std::conj(m_y), std::conj(m_z));
    }

    /*
    Biquaternion q
    norm: q * q.biconjugated()

    (a + ib + jc + kd)(a - ib - jc - kd)
    =   aa  - iab  - jac  - kad
      + iab + bb   - ijbc - ikbd
      + jac - jibc + cc   - jkcd
      + kad - kibd - kjcd + dd
    =   aa  - iab  - jac  - kad
      + iab + bb   - kbc  + jbd
      + jac + kbc  + cc   - icd
      + kad - jbd  + icd  + dd
    = aa + bb + cc + dd
    */
    complex norm() const
    {
        return m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;
    }

    double invariantReal() const
    {
        return m_w.real() * m_w.real() + m_x.imag() * m_x.imag() + m_y.imag() * m_y.imag() + m_z.imag() * m_z.imag();
    }

    double invariantImag() const
    {
        return m_w.imag() * m_w.imag() + m_x.real() * m_x.real() + m_y.real() * m_y.real() + m_z.real() * m_z.real();
    }

    /*
    Biquaternion q(w, x, y, z): only 4 {real, hi, hj, hk}
    Biquaternion g: g.norm() = 1
    Lorentz Transformation: g.biconjugated() * q * g.complexConjugated()
    */
    Biquaternion lorentzTransformation(Biquaternion g) const
    {
        return g.biconjugated() * (*this) * g.complexConjugated();
    }

    std::string toString()
    {
        std::ostringstream o;
        o << m_w << ", " << m_x << ", " << m_y << ", " << m_z;
        return o.str();
    }

private:
    complex m_w;
    complex m_x;
    complex m_y;
    complex m_z;
};

} // namespace
