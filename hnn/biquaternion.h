#pragma once

#include "quaternion.h"
#include "spacetime.h"
#include <complex>

namespace hnn // https://github.com/hypernumbernet
{

using complex = std::complex<double>;

// https://en.wikipedia.org/wiki/Biquaternion
class Biquaternion
{
public:
    Biquaternion() {}

    constexpr Biquaternion(const complex& w, const complex& x, const complex& y, const complex& z)
        : m_w(w), m_x(x), m_y(y), m_z(z) {}

    explicit constexpr Biquaternion(const complex& w)
        : m_w(w), m_x(), m_y(), m_z() {}

    explicit Biquaternion(const Quaternion& q)
        : m_w(q.re(), 0.0), m_x(q.i1(), 0.0), m_y(q.i2(), 0.0), m_z(q.i3(), 0.0) {}

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

    void set(const Biquaternion& bq)
    {
        m_w = bq.w();
        m_x = bq.x();
        m_y = bq.y();
        m_z = bq.z();
    }

    void set(const complex& w, const complex& x, const complex& y, const complex& z)
    {
        m_w = w;
        m_x = x;
        m_y = y;
        m_z = z;
    }

    void setSpacetimeReal(const double* const a)
    {
        m_w.real(a[0]);
        m_x.imag(a[1]);
        m_y.imag(a[2]);
        m_z.imag(a[3]);
    }

    template <typename E>
    void setSpacetimeReal(const double* const a, const E index)
    {
        m_w.real(a[index]);
        m_x.imag(a[index + 1]);
        m_y.imag(a[index + 2]);
        m_z.imag(a[index + 3]);
    }

    void setSpacetimeReal(const Spacetime& q)
    {
        m_w.real(q.w());
        m_x.imag(q.x());
        m_y.imag(q.y());
        m_z.imag(q.z());
    }

    void setSpacetimeImag(const double* const a)
    {
        m_w.imag(a[0]);
        m_x.real(a[1]);
        m_y.real(a[2]);
        m_z.real(a[3]);
    }

    template <typename E>
    void setSpacetimeImag(const double* const a, const E index)
    {
        m_w.imag(a[index]);
        m_x.real(a[index + 1]);
        m_y.real(a[index + 2]);
        m_z.real(a[index + 3]);
    }

    void setSpacetimeImag(const Spacetime& q)
    {
        m_w.imag(q.w());
        m_x.real(q.x());
        m_y.real(q.y());
        m_z.real(q.z());
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
        return m_w.real() * m_w.real() - m_x.imag() * m_x.imag() - m_y.imag() * m_y.imag() - m_z.imag() * m_z.imag();
    }

    double invariantImag() const
    {
        return - m_w.imag() * m_w.imag() + m_x.real() * m_x.real() + m_y.real() * m_y.real() + m_z.real() * m_z.real();
    }

    /*
    Biquaternion q: time-space
    Biquaternion g: g.norm() = 1
    Lorentz Transformation: g.biconjugated() * q * g.complexConjugated()
    */
    Biquaternion lorentzTransformation(const Biquaternion& g) const
    {
        return (g.biconjugated() * (*this)) * g.complexConjugated();
    }

    /*
    For faster calculations
    g  =   ha + ib + jc + kd + p + hiq + hjr + hks
    g* =   ha - ib - jc - kd + p - hiq - hjr - hks
    g~ = - ha + ib + jc + kd + p - hiq - hjr - hks
    f = w + hix + hjy + hkz

    g* f
    = (ha - ib - jc - kd + p - hiq - hjr - hks)(w + hix + hjy + hkz)

    = h  aw + hhi  ax + hhj  ay + hhk  az - i  bw - ihi  bx - ihj  by - ihk  bz
    - j  cw - jhi  cx - jhj  cy - jhk  cz - k  dw - khi  dx - khj  dy - khk  dz
    +    pw + hi   px + hj   py + hk   pz - hi qw - hihi qx - hihj qy - hihk qz
    - hj rw - hjhi rx - hjhj ry - hjhk rz - hk sw - hkhi sx - hkhj sy - hkhk sz

    = h  aw - i  ax - j  ay - k  az - i  bw + h  bx - hk by + hj bz
    - j  cw + hk cx + h  cy - hi cz - k  dw - hj dx + hi dy + h  dz
    +    pw + hi px + hj py + hk pz - hi qw -    qx + k  qy - j  qz
    - hj rw - k  rx -    ry + i  rz - hk sw + j  sx - i  sy -    sz

    = h (  aw + bx + cy + dz)
    + i (- ax - bw + rz - sy)
    + j (- ay - cw - qz + sx)
    + k (- az - dw + qy - rx)
    +      pw - qx - ry - sz
    + hi(- cz + dy + px - qw)
    + hj(  bz - dx + py - rw)
    + hk(- by + cx + pz - sw)

    g* f g~
    = g* f (- ha + ib + jc + kd + p - hiq - hjr - hks)

    h (  aw + bx + cy + dz)(- ha + ib + jc + kd + p - hiq - hjr - hks) =
    +   (  aaw + abx + acy + adz)
    + hi(  abw + bbx + bcy + bdz)
    + hj(  acw + bcx + ccy + cdz)
    + hk(  adw + bdx + cdy + ddz)
    + h (  apw + bpx + cpy + dpz)
    + i (  aqw + bqx + cqy + dqz)
    + j (  arw + brx + cry + drz)
    + k (  asw + bsx + csy + dsz)

    i (- ax - bw + rz - sy)(- ha + ib + jc + kd + p - hiq - hjr - hks) =
    - hi(- aax - abw + arz - asy)
    -   (- abx - bbw + brz - bsy)
    + k (- acx - bcw + crz - csy)
    - j (- adx - bdw + drz - dsy)
    + i (- apx - bpw + prz - psy)
    + h (- aqx - bqw + qrz - qsy)
    - hk(- arx - brw + rrz - rsy)
    + hj(- asx - bsw + rsz - ssy)
    =
    + hi(  aax + abw - arz + asy)
    +   (  abx + bbw - brz + bsy)
    + k (- acx - bcw + crz - csy)
    + j (  adx + bdw - drz + dsy)
    + i (- apx - bpw + prz - psy)
    + h (- aqx - bqw + qrz - qsy)
    + hk(  arx + brw - rrz + rsy)
    + hj(- asx - bsw + rsz - ssy)

    j (- ay - cw - qz + sx)(- ha + ib + jc + kd + p - hiq - hjr - hks) =
    - hj(- aay - acw - aqz + asx)
    - k (- aby - bcw - bqz + bsx)
    -   (- acy - ccw - cqz + csx)
    + i (- ady - cdw - dqz + dsx)
    + j (- apy - cpw - pqz + psx)
    + hk(- aqy - cqw - qqz + qsx)
    + h (- ary - crw - qrz + rsx)
    - hi(- asy - csw - qsz + ssx)
    =
    + hj(  aay + acw + aqz - asx)
    + k (  aby + bcw + bqz - bsx)
    +   (  acy + ccw + cqz - csx)
    + i (- ady - cdw - dqz + dsx)
    + j (- apy - cpw - pqz + psx)
    + hk(- aqy - cqw - qqz + qsx)
    + h (- ary - crw - qrz + rsx)
    + hi(  asy + csw + qsz - ssx)

    k (- az - dw + qy - rx)(- ha + ib + jc + kd + p - hiq - hjr - hks) =
    - hk(- aaz - adw + aqy - arx)
    + j (- abz - bdw + bqy - brx)
    - i (- acz - cdw + cqy - crx)
    -   (- adz - ddw + dqy - drx)
    + k (- apz - dpw + pqy - prx)
    - hj(- aqz - dqw + qqy - qrx)
    + hi(- arz - drw + qry - rrx)
    + h (- asz - dsw + qsy - rsx)
    =
    + hk(  aaz + adw - aqy + arx)
    + j (- abz - bdw + bqy - brx)
    + i (  acz + cdw - cqy + crx)
    +   (  adz + ddw - dqy + drx)
    + k (- apz - dpw + pqy - prx)
    + hj(  aqz + dqw - qqy + qrx)
    + hi(- arz - drw + qry - rrx)
    - h (  asz + dsw - qsy + rsx)

    (pw - qx - ry - sz)(- ha + ib + jc + kd + p - hiq - hjr - hks) =
    - h (apw - aqx - ary - asz)
    + i (bpw - bqx - bry - bsz)
    + j (cpw - cqx - cry - csz)
    + k (dpw - dqx - dry - dsz)
    +   (ppw - pqx - pry - psz)
    - hi(pqw - qqx - qry - qsz)
    - hj(prw - qrx - rry - rsz)
    - hk(psw - qsx - rsy - ssz)
    =
    + h (- apw + aqx + ary + asz)
    + i (  bpw - bqx - bry - bsz)
    + j (  cpw - cqx - cry - csz)
    + k (  dpw - dqx - dry - dsz)
    +   (  ppw - pqx - pry - psz)
    + hi(- pqw + qqx + qry + qsz)
    + hj(- prw + qrx + rry + rsz)
    + hk(- psw + qsx + rsy + ssz)

    hi(- cz + dy + px - qw)(- ha + ib + jc + kd + p - hiq - hjr - hks) =
    + i (- acz + ady + apx - aqw)
    - h (- bcz + bdy + bpx - bqw)
    + hk(- ccz + cdy + cpx - cqw)
    - hj(- cdz + ddy + dpx - dqw)
    + hi(- cpz + dpy + ppx - pqw)
    -   (- cqz + dqy + pqx - qqw)
    + k (- crz + dry + prx - qrw)
    - j (- csz + dsy + psx - qsw)
    =
    + i (- acz + ady + apx - aqw)
    + h (  bcz - bdy - bpx + bqw)
    + hk(- ccz + cdy + cpx - cqw)
    + hj(  cdz - ddy - dpx + dqw)
    + hi(- cpz + dpy + ppx - pqw)
    +   (  cqz - dqy - pqx + qqw)
    + k (- crz + dry + prx - qrw)
    + j (  csz - dsy - psx + qsw)

    hj(  bz - dx + py - rw)(- ha + ib + jc + kd + p - hiq - hjr - hks) =
    + j (  abz - adx + apy - arw)
    - hk(  bbz - bdx + bpy - brw)
    - h (  bcz - cdx + cpy - crw)
    + hi(  bdz - ddx + dpy - drw)
    + hj(  bpz - dpx + ppy - prw)
    - k (  bqz - dqx + pqy - qrw)
    -   (  brz - drx + pry - rrw)
    + i (  bsz - dsx + psy - rsw)
    =
    + j (  abz - adx + apy - arw)
    + hk(- bbz + bdx - bpy + brw)
    + h (- bcz + cdx - cpy + crw)
    + hi(  bdz - ddx + dpy - drw)
    + hj(  bpz - dpx + ppy - prw)
    + k (- bqz + dqx - pqy + qrw)
    +   (- brz + drx - pry + rrw)
    + i (  bsz - dsx + psy - rsw)

    hk(- by + cx + pz - sw)(- ha + ib + jc + kd + p - hiq - hjr - hks) =
    + k (- aby + acx + apz - asw)
    + hj(- bby + bcx + bpz - bsw)
    - hi(- bcy + ccx + cpz - csw)
    - h (- bdy + cdx + dpz - dsw)
    + hk(- bpy + cpx + ppz - psw)
    + j (- bqy + cqx + pqz - qsw)
    - i (- bry + crx + prz - rsw)
    -   (- bsy + csx + psz - ssw)
    =
    + k (- aby + acx + apz - asw)
    + hj(- bby + bcx + bpz - bsw)
    + hi(  bcy - ccx - cpz + csw)
    + h (  bdy - cdx - dpz + dsw)
    + hk(- bpy + cpx + ppz - psw)
    + j (- bqy + cqx + pqz - qsw)
    + i (  bry - crx - prz + rsw)
    +   (  bsy - csx - psz + ssw)

    g* f g~ =
    + h (apw + bpx + cpy + dpz - aqx - bqw + qrz - qsy
       - ary - crw - qrz + rsx - asz - dsw + qsy - rsx
       - apw + aqx + ary + asz + bcz - bdy - bpx + bqw
       - bcz + cdx - cpy + crw + bdy - cdx - dpz + dsw)
    + i (aqw + bqx + cqy + dqz - apx - bpw + prz - psy
       - ady - cdw - dqz + dsx + acz + cdw - cqy + crx
       + bpw - bqx - bry - bsz - acz + ady + apx - aqw
       + bsz - dsx + psy - rsw + bry - crx - prz + rsw)
    + j (arw + brx + cry + drz + adx + bdw - drz + dsy
       - apy - cpw - pqz + psx - abz - bdw + bqy - brx
       + cpw - cqx - cry - csz + csz - dsy - psx + qsw
       + abz - adx + apy - arw - bqy + cqx + pqz - qsw)
    + k (asw + bsx + csy + dsz - acx - bcw + crz - csy
       + aby + bcw + bqz - bsx - apz - dpw + pqy - prx
       + dpw - dqx - dry - dsz - crz + dry + prx - qrw
       - bqz + dqx - pqy + qrw - aby + acx + apz - asw)
    +   (aaw + abx + acy + adz + abx + bbw - brz + bsy
       + acy + ccw + cqz - csx + adz + ddw - dqy + drx
       + ppw - pqx - pry - psz + cqz - dqy - pqx + qqw
       - brz + drx - pry + rrw + bsy - csx - psz + ssw)
    + hi(abw + bbx + bcy + bdz + aax + abw - arz + asy
       + asy + csw + qsz - ssx - arz - drw + qry - rrx
       - pqw + qqx + qry + qsz - cpz + dpy + ppx - pqw
       + bdz - ddx + dpy - drw + bcy - ccx - cpz + csw)
    + hj(acw + bcx + ccy + cdz - asx - bsw + rsz - ssy
       + aay + acw + aqz - asx + aqz + dqw - qqy + qrx
       - prw + qrx + rry + rsz + cdz - ddy - dpx + dqw
       + bpz - dpx + ppy - prw - bby + bcx + bpz - bsw)
    + hk(adw + bdx + cdy + ddz + arx + brw - rrz + rsy
       - aqy - cqw - qqz + qsx + aaz + adw - aqy + arx
       - psw + qsx + rsy + ssz - ccz + cdy + cpx - cqw
       - bbz + bdx - bpy + brw - bpy + cpx + ppz - psw)
    =
    + h (- 2 qsy + 2 rsx + 2 asz + 2 dsw)
    +   (aaw + bbw + ccw + ddw + ppw + qqw + rrw + ssw
       + 2 abx - 2 csx - 2 pqx + 2 drx + 2 acy - 2 dqy
       - 2 pry + 2 bsy + 2 adz - 2 brz + 2 cqz - 2 psz)
    + hi(aax + bbx - ccx - ddx + ppx + qqx - rrx - ssx
       + 2 abw + 2 bdz + 2 asy + 2 csw + 2 qsz + 2 bcy
       - 2 pqw - 2 cpz + 2 dpy - 2 arz - 2 drw + 2 qry)
    + hj(aay - bby + ccy - ddy + ppy - qqy + rry - ssy
       + 2 acw + 2 bcx + 2 cdz - 2 asx - 2 bsw + 2 rsz
       - 2 prw - 2 dpx + 2 bpz + 2 aqz + 2 dqw + 2 qrx)
    + hk(aaz - bbz - ccz + ddz + ppz - qqz - rrz + ssz
       + 2 adw + 2 bdx + 2 cdy + 2 arx + 2 brw + 2 rsy
       - 2 bpy - 2 aqy - 2 cqw + 2 qsx - 2 psw + 2 cpx)
    =
    + (aa + bb + cc + dd + pp + qq + rr + ss)w
      + 2((ab - cs - pq + dr)x + (ac - dq - pr + bs)y + (ad - br + cq - ps)z)
    + hi((aa + bb - cc - dd + pp + qq - rr - ss)x
      + 2((ab + cs - pq - dr)w + (as + bc + dp + qr)y + (bd + qs - cp - ar)z))
    + hj((aa - bb + cc - dd + pp - qq + rr - ss)y
      + 2((ac - bs - pr + dq)w + (bc - as - dp + qr)x + (cd + rs + bp + aq)z))
    + hk((aa - bb - cc + dd + pp - qq - rr + ss)z
      + 2((ad + br - cq - ps)w + (bd + ar + cp + qs)x + (cd + rs - bp - aq)y))
    */
    Biquaternion lorentzTransformation(const double w, const double x, const double y, const double z) const
    {
        const double a = h();
        const double b = i();
        const double c = j();
        const double d = k();
        const double p = re();
        const double q = hi();
        const double r = hj();
        const double s = hk();
        return Biquaternion(
            (a * a + b * b + c * c + d * d + p * p + q * q + r * r + s * s) * w
            + 2.0 * ((a * b - c * s - p * q + d * r) * x + (a * c - d * q - p * r + b * s) * y + (a * d - b * r + c * q - p * s) * z),
            0.0,
            0.0,
            (a * a + b * b - c * c - d * d + p * p + q * q - r * r - s * s) * x
            + 2.0 * ((a * b + c * s - p * q - d * r) * w + (a * s + b * c + d * p + q * r) * y + (b * d + q * s - c * p - a * r) * z),
            0.0,
            (a * a - b * b + c * c - d * d + p * p - q * q + r * r - s * s) * y
            + 2.0 * ((a * c - b * s - p * r + d * q) * w + (b * c - a * s - d * p + q * r) * x + (c * d + r * s + b * p + a * q) * z),
            0.0,
            (a * a - b * b - c * c + d * d + p * p - q * q - r * r + s * s) * z
            + 2.0 * ((a * d + b * r - c * q - p * s) * w + (b * d + a * r + c * p + q * s) * x + (c * d + r * s - b * p - a * q) * y)
        );
    }

    void lorentzTransformation(double& mw, double& mx, double& my, double& mz) const
    {
        const double a = h();
        const double b = i();
        const double c = j();
        const double d = k();
        const double p = re();
        const double q = hi();
        const double r = hj();
        const double s = hk();
        const double w = mw;
        const double x = mx;
        const double y = my;
        const double z = mz;
        mw = (a * a + b * b + c * c + d * d + p * p + q * q + r * r + s * s) * w
            + 2.0 * ((a * b - c * s - p * q + d * r) * x + (a * c - d * q - p * r + b * s) * y + (a * d - b * r + c * q - p * s) * z);
        mx = (a * a + b * b - c * c - d * d + p * p + q * q - r * r - s * s) * x
            + 2.0 * ((a * b + c * s - p * q - d * r) * w + (a * s + b * c + d * p + q * r) * y + (b * d + q * s - c * p - a * r) * z);
        my = (a * a - b * b + c * c - d * d + p * p - q * q + r * r - s * s) * y
            + 2.0 * ((a * c - b * s - p * r + d * q) * w + (b * c - a * s - d * p + q * r) * x + (c * d + r * s + b * p + a * q) * z);
        mz = (a * a - b * b - c * c + d * d + p * p - q * q - r * r + s * s) * z
            + 2.0 * ((a * d + b * r - c * q - p * s) * w + (b * d + a * r + c * p + q * s) * x + (c * d + r * s - b * p - a * q) * y);
    }

    /**
     * @brief Lorentz Transformation
     * @param qu: Quaternion only used for time-space 4 vector
     */
    void lorentzTransformation(Quaternion& qu) const
    {
        const double a = h();
        const double b = i();
        const double c = j();
        const double d = k();
        const double p = re();
        const double q = hi();
        const double r = hj();
        const double s = hk();
        const double w = qu.re();
        const double x = qu.i1();
        const double y = qu.i2();
        const double z = qu.i3();
        qu.set(
            (a * a + b * b + c * c + d * d + p * p + q * q + r * r + s * s) * w
            + 2.0 * ((a * b - c * s - p * q + d * r) * x + (a * c - d * q - p * r + b * s) * y + (a * d - b * r + c * q - p * s) * z),
            (a * a + b * b - c * c - d * d + p * p + q * q - r * r - s * s) * x
            + 2.0 * ((a * b + c * s - p * q - d * r) * w + (a * s + b * c + d * p + q * r) * y + (b * d + q * s - c * p - a * r) * z),
            (a * a - b * b + c * c - d * d + p * p - q * q + r * r - s * s) * y
            + 2.0 * ((a * c - b * s - p * r + d * q) * w + (b * c - a * s - d * p + q * r) * x + (c * d + r * s + b * p + a * q) * z),
            (a * a - b * b - c * c + d * d + p * p - q * q - r * r + s * s) * z
            + 2.0 * ((a * d + b * r - c * q - p * s) * w + (b * d + a * r + c * p + q * s) * x + (c * d + r * s - b * p - a * q) * y)
        );
    }

private:
    complex m_w;
    complex m_x;
    complex m_y;
    complex m_z;
};

inline std::ostream& operator<<(std::ostream& os, const Biquaternion& bq)
{
    os << bq.w() << ", " << bq.x() << ", " << bq.y() << ", " << bq.z();
    return os;
}

constexpr bool fuzzyCompare(const Biquaternion& bq1, const Biquaternion& bq2)
{
    return fuzzyCompare(bq1.re(), bq2.re()) && fuzzyCompare(bq1.i(), bq2.i()) && fuzzyCompare(bq1.j(), bq2.j()) && fuzzyCompare(bq1.k(), bq2.k());
}

} // namespace
