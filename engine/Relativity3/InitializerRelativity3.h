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
        T* const momentums = m_engine->velocities();
        T* const masses = m_engine->masses();

        for (quint64 i = 0; i < num; ++i)
        {
            const quint64 i3 = i * 3;
            Vector3 p(momentums[i3], momentums[i3 + 1], momentums[i3 + 2]);
            p *= masses[i];
            momentums[i3    ] = p.x();
            momentums[i3 + 1] = p.y();
            momentums[i3 + 2] = p.z();
        }
    }

    AbstractNBodyEngine<T>* const m_engine;

    Initializer3D<T> m_3d;
};
