#pragma once

#include "initializer3d.h"
#include "hnn/quaternion.h"

using namespace hnn;

template <typename T>
class Initializer4D3D : protected AbstractInitializer
{
public:
    Initializer4D3D(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : AbstractInitializer(sim)
        , m_engine(engine)
        , m_3d(sim, engine)
    {
    }

private:
    void initRandamCube() override;
    void initRandamSphere(double) override;
    void initSunEarth() override;
    void initEarthSun() override;
    void initEarthMoon() override;
    void initSunEarthVenus() override;
    void initTestSamePosition() override;
    void initSunMercury() override;

    inline void fromInitializer3D()
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

            auto q = Quaternion<T>::exp({coordinates, i3});
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
