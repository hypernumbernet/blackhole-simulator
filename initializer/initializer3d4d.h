#pragma once

#include "initializer3d.h"
#include "quaternion.h"

using namespace bhs;

template <typename T>
class Initializer3D4D : public AbstractInitializer
{
public:
    Initializer3D4D(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : AbstractInitializer(sim)
        , m_engine(engine)
        , m_3d(sim, engine)
    {
    }

protected:
    void initRandamCube() override;
    void initRandamSphere(double) override;
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
        const auto angle = m_engine->velocityToAngle(speed);
        return bhs::Quaternion<T>::Exp(dr * angle);
    }

    inline bhs::Quaternion<T> fromVector3(bhs::Vector3<T> v3) const
    {
        const auto speed = v3.Abs();
        return fromDirectionAndSpeed(v3.x, v3.y, v3.z, speed);
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
            embedQuaternionToArray<T>(q, vels, i4);
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
