#pragma once

#include "engine/3D/Initializer3D.h"

using namespace hnn;

template <typename T>
class InitializerRelativity2 : protected AbstractInitializer
{
public:
    InitializerRelativity2(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
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
        T* const angles = m_engine->velocities();
        T* const masses = m_engine->masses();
        const double speedOfLight = m_engine->scaleInv() * SPEED_OF_LIGHT;

        for (quint64 i = 0; i < num; ++i)
        {
            const quint64 i3 = i * 3;
            Vector3 p(angles[i3], angles[i3 + 1], angles[i3 + 2]);
            p *= masses[i];
            const Vector3 a = Spacetime::versorAngle(p, masses[i], speedOfLight);
            angles[i3    ] = a.x();
            angles[i3 + 1] = a.y();
            angles[i3 + 2] = a.z();
        }
    }

    AbstractNBodyEngine<T>* const m_engine;

    Initializer3D<T> m_3d;
};
