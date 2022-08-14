#pragma once

#include "hnn/quaternion.h"
#include "initializer3d.h"

using namespace hnn;

template <typename T>
class Initializer3D4D : protected AbstractInitializer
{
public:
    Initializer3D4D(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : AbstractInitializer(sim)
        , m_engine(engine)
        , m_3d(sim, engine)
    {
    }

private:
    void initRandamCube() override;
    void initRandamSphere(double) override;
    void initCustom() override;
    void initSunEarth() override;
    void initEarthSun() override;
    void initEarthMoon() override;
    void initSunEarthVenus() override;
    void initTestSamePosition() override;
    void initSunMercury() override;

    inline Quaternion<T> fromDirectionAndSpeed(
            const T x, const T y, const T z,
            const T speed
            ) const
    {
        Vector3<T> dr(x, y, z);
        dr.normalize();
        const auto angle = m_engine->velocityToAngle(speed);
        return Quaternion<T>::exp(dr * angle);
    }

    inline Quaternion<T> fromVector3(Vector3<T> v3) const
    {
        const auto speed = v3.abs();
        return fromDirectionAndSpeed(v3.x(), v3.y(), v3.z(), speed);
    }

    inline void fromInitializer3D()
    {
        const quint64 num = m_engine->numberOfParticle();
        T* const velocities = m_engine->velocities();

        T* vels = new T[num * 4];

        for (quint64 i = 0; i < num; ++i)
        {
            quint64 i3 = i * 3;
            quint64 i4 = i * 4;

            auto q = fromVector3({velocities, i3});
            bhs::embedQuaternionToArray<T>(q, vels, i4);
        }
        for (quint64 i = 0; i < num * 4; ++i)
        {
            velocities[i] = vels[i];
        }

        delete[] vels;
    }

    AbstractNBodyEngine<T>* const m_engine;

    Initializer3D<T> m_3d;
};
