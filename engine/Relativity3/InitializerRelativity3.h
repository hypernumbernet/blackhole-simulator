#pragma once

#include "engine/3d/initializer3d.h"

using namespace hnn;

template <typename T>
class InitializerRelativity3 : protected AbstractInitializer
{
public:
    InitializerRelativity3(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : AbstractInitializer(sim)
        , m_engine(engine)
        , m_3d(sim, engine)
    {
    }

private:
    void initRandomCube() override;
    void initRandomSphere(double) override;
    void initCustom() override;

    void fromInitializer3D()
    {
        const quint64 num = m_engine->numberOfParticle();
        T* const velocities = m_engine->velocities();
        T* vels = new T[num * 4]();
        for (quint64 i = 0; i < num; ++i)
        {
            const quint64 i3 = i * 3;
            const quint64 i4 = i * 4;
            Vector3 v3((double)velocities[i3], (double)velocities[i3 + 1], (double)velocities[i3 + 2]);
            const double speed = v3.abs();
            if (speed != 0.)
            {
                v3 /= speed;
            }
            const auto angle = m_engine->velocityToAngle(speed);
            auto q = Quaternion::exp(v3 * angle);
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
