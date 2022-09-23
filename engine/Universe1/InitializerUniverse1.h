#pragma once

#include "hnn/quaternion.h"
#include "engine/3D/Initializer3D.h"

using namespace hnn;

template <typename T>
class InitializerUniverse1 : protected AbstractInitializer
{
public:
    InitializerUniverse1(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : AbstractInitializer(sim)
        , m_engine(engine)
        , m_3d(sim, engine)
    {
    }

private:
    void initRandomCube() override;
    void initRandomSphere(double) override;
    void initCustom() override;

    Quaternion fromDirectionAndSpeed(
            const double x, const double y, const double z,
            const double speed
            ) const
    {
        Vector3 dr(x, y, z);
        dr.normalize();
        const auto angle = m_engine->velocityToAngle(speed);
        return Quaternion::exp(dr * angle);
    }

    Quaternion fromVector3(Vector3 v3) const
    {
        const auto speed = v3.abs();
        return fromDirectionAndSpeed(v3.x(), v3.y(), v3.z(), speed);
    }

    void fromInitializer3D()
    {
        const quint64 num = m_engine->numberOfParticle();
        T* const velocities = m_engine->velocities();

        T* vels = new T[num * 4];

        for (quint64 i = 0; i < num; ++i)
        {
            quint64 i3 = i * 3;
            quint64 i4 = i * 4;

            auto q = fromVector3({velocities[i3], velocities[i3+1], velocities[i3+2]});
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
