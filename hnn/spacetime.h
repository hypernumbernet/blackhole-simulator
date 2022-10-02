#pragma once

#include "constants.h"
#include "vector3.h"

namespace hnn // https://github.com/hypernumbernet
{

using complex = std::complex<double>;

/**
 * @brief The Spacetime class
 * The part of Biquaternion {real, hi, hj, hk}
 * https://en.wikipedia.org/wiki/Biquaternion
 */
class Spacetime
{
public:
    Spacetime() {}

    constexpr Spacetime(const double w, const double x, const double y, const double z)
        : m_w(w), m_x(x), m_y(y), m_z(z) {}

    explicit Spacetime(const double w)
        : m_w(w), m_x(0.0), m_y(0.0), m_z(0.0) {}

    explicit Spacetime(const Vector3& v)
        : m_w(0.0), m_x(v.x()), m_y(v.y()), m_z(v.z()) {}

    template <typename E>
    Spacetime(const double* const a, const E index)
        : m_w(a[index]), m_x(a[index + 1]), m_y(a[index + 2]), m_z(a[index + 3]) {}

    double w() const { return m_w; }
    double x() const { return m_x; }
    double y() const { return m_y; }
    double z() const { return m_z; }

    void setW(const double w) { m_w = w; }
    void setX(const double x) { m_x = x; }
    void setY(const double y) { m_y = y; }
    void setZ(const double z) { m_z = z; }

    void set(const Spacetime& st)
    {
        m_w = st.m_w;
        m_x = st.m_x;
        m_y = st.m_y;
        m_z = st.m_z;
    }

    void set(const double w, const double x, const double y, const double z)
    {
        m_w = w;
        m_x = x;
        m_y = y;
        m_z = z;
    }

    void set(const double w)
    {
        m_w = w;
        m_x = 0.;
        m_y = 0.;
        m_z = 0.;
    }

    void set(const double* const a)
    {
        m_w = a[0];
        m_x = a[1];
        m_y = a[2];
        m_z = a[3];
    }

    template <typename E>
    void set(const double* const a, const E index)
    {
        m_w = a[index];
        m_x = a[index + 1];
        m_y = a[index + 2];
        m_z = a[index + 3];
    }

    static constexpr Spacetime zero()
    {
        return Spacetime(0., 0., 0., 0.);
    }

    static constexpr Spacetime identity()
    {
        return Spacetime(1., 0., 0., 0.);
    }

    bool operator==(const Spacetime& a) const
    {
        return m_w == a.m_w && m_x == a.m_x && m_y == a.m_y && m_z == a.m_z;
    }

    bool operator!=(const Spacetime& a) const
    {
        return m_w != a.m_x || m_x != a.m_x || m_y != a.m_y || m_z != a.m_z;
    }

    const double& operator[](const int i) const
    {
        if (i == 0) return m_w;
        if (i == 1) return m_x;
        if (i == 2) return m_y;
        return m_z;
    }

    double& operator[](const int i)
    {
        if (i == 0) return m_w;
        if (i == 1) return m_x;
        if (i == 2) return m_y;
        return m_z;
    }

    // Invariant
    double norm() const
    {
        return m_w * m_w - m_x * m_x - m_y * m_y - m_z * m_z;
    }

    Spacetime conjugated() const
    {
        return Spacetime(m_w, - m_x, - m_y, - m_z);
    }

    complex abs() const
    {
        return sqrt(complex(norm(), 0.));
    }

    // Rapidity
    // {a : R : arg() }
    // v = c tanh(a)
    // exp(h a r) = cosh(a) + h r sinh(a)
    //            = w + hi x + hj y + hk z
    // {h : a biquaterinon unit : hh = -1}
    // {r = i x + j y + k z : xx + yy + zz = 1}
    // When your spacetime is from Exponential
    // simply arg() := acosh(w) { w >= 1 }
    double arg() const
    {
        const double n = sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
        return atanh(n / m_w);
    }

    // Exponential - Hyperbolic
    static Spacetime exp(const double x, const double y, const double z)
    {
        const double n = sqrt(x * x + y * y + z * z);
        if (n == 0.0)
            return identity();
        const double a = sinh(n) / n;
        return Spacetime(cosh(n), x * a, y * a, z * a);
    }

    // Exponential - Hyperbolic
    // a = atanh(v/c)
    // Vector3(x, y, z).norm() = 1
    static Spacetime exp(const double a, const double x, const double y, const double z)
    {
        const double s = sinh(a);
        return Spacetime(cosh(a), x * s, y * s, z * s);
    }

    // Exponential - Hyperbolic
    // v.norm() = 1
    static Spacetime exp(const double a, const Vector3& v)
    {
        const double s = sinh(a);
        return Spacetime(cosh(a), v.x() * s, v.y() * s, v.z() * s);
    }

    static Vector3 versorAngle(const Vector3& v, const double speedOfLightInv)
    {
        const double speed = v.abs();
        if (speed == 0.)
            return Vector3::zero();
        const double a = atanh(speed * speedOfLightInv);
        const double b = a / speed;
        return Vector3(b * v.x(), b * v.y(), b * v.z());
    }

    /**
     * @brief versor angles from momentum
     * p = mvγ
     * {γ : Lorentz factor : 1 / sqrt(1 - v^2 / c^2)}
     *
     * p = mv / sqrt(1 - v^2 / c^2)
     * p^2 (1 -  v^2 / c^2)= m^2 v^2
     * m^2 v^2 + p^2 v^2 / c^2 = p^2
     * (m^2 + p^2 / c^2) v^2 = p^2
     *
     * v = p / sqrt(m^2 + |p|^2 / c^2 )
     * |v| -> c (|p| -> ∞, m -> ∞)
     * |v| = c (m = 0)
     *
     * v / c = p / sqrt(m^2 c^2 + |p|^2) = tanh(a) = pc / E
     * tanh(a) < 1 (|p| -> ∞)
     *
     * @param p momentum
     * @param speed of light on your scale
     * @return versor angles
     */
    static Vector3 versorAngle(const Vector3& p, const double m, const double speedOfLight)
    {
        const double pn = p.norm();
        if (pn == 0.)
            return Vector3::zero();
        const double pr = sqrt(pn);
        const double l = pr / sqrt(m * m * speedOfLight * speedOfLight + pn);
        const double a = atanh(l);
        const double b = a / pr;
        return Vector3(b * p.x(), b * p.y(), b * p.z());
    }

    static Vector3 velocities(const Vector3& versorAngle, const double speedOfLight)
    {
        const double a = versorAngle.abs();
        if (a == 0.)
            return Vector3::zero();
        const double beta = tanh(a);
        const double v = beta * speedOfLight / a;
        return Vector3(v * versorAngle.x(), v * versorAngle.y(), v * versorAngle.z());
    }

    /**
     * @brief Lorentz Transformation by Biquaternion
     * @param g
     * {g  : exp(- 0.5 h a r)
     * {g* : biconjugation}
     * {g~ : complex conjugation}
     * g* = g~
     * T(spacetime) = g* spacetime g~
     */
    void lorentzTransformation(const Spacetime& g)
    {
        const double p = g.m_w;
        const double q = g.m_x;
        const double r = g.m_y;
        const double s = g.m_z;
        const double w = m_w;
        const double x = m_x;
        const double y = m_y;
        const double z = m_z;
        m_w = (p * p + q * q + r * r + s * s) * w + 2. * p * ( - q * x - r * y - s * z);
        m_x = (p * p + q * q - r * r - s * s) * x + 2. * q * ( - p * w + r * y + s * z);
        m_y = (p * p - q * q + r * r - s * s) * y + 2. * r * ( - p * w + q * x + s * z);
        m_z = (p * p - q * q - r * r + s * s) * z + 2. * s * ( - p * w + q * x + r * y);
    }

    void lorentzTransformation(const Vector3& v, const double speedOfLightInv)
    {
        const double l = v.abs();
        if (l == 0.)
            return;
        const double a = atanh(l * speedOfLightInv);
        const Vector3 dir = v / l;
        const Spacetime g = Spacetime::exp(- 0.5 * a, dir);
        lorentzTransformation(g);
    }

    void lorentzTransformation(const Vector3& versorAngle)
    {
        const double a = versorAngle.abs();
        if (a == 0.)
            return;
        const Vector3 dir = versorAngle / a;
        const Spacetime g = Spacetime::exp(- 0.5 * a, dir);
        lorentzTransformation(g);
    }

    bool fuzzyCompare(const Spacetime& a)
    {
        return hnn::fuzzyCompare(m_w, a.m_w) &&
               hnn::fuzzyCompare(m_x, a.m_x) &&
               hnn::fuzzyCompare(m_y, a.m_y) &&
               hnn::fuzzyCompare(m_z, a.m_z);
    }

    friend std::ostream& operator<<(std::ostream& os, const Spacetime& st)
    {
        os << st.m_w << ", " << st.m_x << ", " << st.m_y << ", " << st.m_z;
        return os;
    }

private:
    double m_w;
    double m_x;
    double m_y;
    double m_z;
};

} // namespace
