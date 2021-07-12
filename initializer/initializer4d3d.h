#pragma once

#include "initializer3d.h"
#include "quaternion.h"

using namespace bhs;

template <typename T>
class Initializer4D3D : public AbstractInitializer
{
public:
    Initializer4D3D(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
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

    inline void fromInitializer3D()
    {
        const quint64 num = m_engine->numberOfParticle();
        const T* const coordinates = m_engine->coordinates();
        T* const locations = m_engine->locations();

        for (quint64 i = 0; i < num; ++i)
        {
            quint64 i3 = i * 3;
            quint64 i4 = i * 4;

            auto q = Quaternion<T>::Exp({coordinates, i3});
            embedQuaternionToArray(q, locations, i4);
        }
    }

    AbstractNBodyEngine<T>* const m_engine;

    Initializer3D<T> m_3d;
};
