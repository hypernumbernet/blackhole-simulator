#pragma once

#include "abstractinitializer.h"
#include "quaternion.h"

using namespace bhs;

template <typename T>
class Initializer3D4D : public AbstractInitializer<T>
{
public:
    using AbstractInitializer<T>::AbstractInitializer;

protected:
    void initRandamCube() override;
    void initRandamSphere(T) override;
    void initSunEarth() override;
    void initEarthSun() override;
    void initEarthMoon() override;
    void initSunEarthVenus() override;
    void initTestSamePosition() override;

private:
    inline bhs::Quaternion<T> fromDirectionAndSpeed(
            const T x, const T y, const T z,
            const T speed
            ) const
    {
        bhs::Vector3<T> dr(x, y, z);
        dr.Normalize();
        const auto angle = speed * AbstractNBodyEngine<T>::VANGLE;
        return bhs::Quaternion<T>::Exp(dr * angle);
    }

    inline void embedQuaternionToArray(
            const bhs::Quaternion<T>& q, T* const a, const quint64 index)
    {
        a[index    ] = q.i0;
        a[index + 1] = q.i1;
        a[index + 2] = q.i2;
        a[index + 3] = q.i3;
    }
};
