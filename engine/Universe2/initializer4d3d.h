#pragma once

#include "hnn/quaternion.h"
#include "engine/3D/Initializer3D.h"

using namespace hnn;

template <typename T>
class InitializerUniverse2 : protected AbstractInitializer
{
public:
    InitializerUniverse2(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
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
        const T* const coordinates = m_engine->coordinates();
        T* const locations = m_engine->locations();
        T* const velocities = m_engine->velocities();
        T time = m_engine->timePerFrame();

        for (quint64 i = 0; i < num; ++i)
        {
            quint64 i3 = i * 3;
            quint64 i4 = i * 4;

            auto q = Quaternion::exp({coordinates[i3],coordinates[i3+1],coordinates[i3+2]});
            bhs::embedQuaternionToArray<T>(q, locations, i4);
        }

        for (quint64 i = 0; i < num * 3; ++i)
        {
            velocities[i] *= time;
        }
    }

    AbstractNBodyEngine<T>* const m_engine;

    Initializer3D<T> m_3d;
};
