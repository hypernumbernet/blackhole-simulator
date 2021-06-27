#pragma once

#include "abstractinitializer.h"
#include "quaternion.h"

class Initializer3D3S : public AbstractInitializer
{
public:
    using AbstractInitializer::AbstractInitializer;

protected:
    void initRandamCube() override;
    void initRandamSphere() override;
    void initSunEarth() override;
    void initEarthSun() override;
    void initEarthMoon() override;
    void initSunEarthVenus() override;
    void initTestSamePosition() override;

private:
    inline bhs::Quaternion<float> fromDirectionAndSpeed(
            const float x, const float y, const float z,
            const float speed
            ) const
    {
        bhs::Vector3<float> dr(x, y, z);
        dr.Normalize();
        const auto angle = speed * AbstractNBodyEngine::VANGLE;
        return bhs::Quaternion<float>::Exp(dr * angle);
    }

    inline void embedQuaternionToArray(
            const bhs::Quaternion<float>& q, float* const a, const quint64 index)
    {
        a[index] = q.i0;
        a[index + 1] = q.i1;
        a[index + 2] = q.i2;
        a[index + 3] = q.i3;
    }
};
