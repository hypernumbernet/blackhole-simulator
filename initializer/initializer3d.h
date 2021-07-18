#pragma once

#include "abstractinitializer.h"
#include "hnn/quaternion.h"

template <typename T> class Initializer3D4D;
template <typename T> class Initializer3D4DR1;
template <typename T> class Initializer4D3D;

template <typename T>
class Initializer3D : protected AbstractInitializer
{
    friend Initializer3D4D<T>;
    friend Initializer3D4DR1<T>;
    friend Initializer4D3D<T>;

public:

    Initializer3D(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : AbstractInitializer(sim)
        , m_engine(engine)
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

    void setRotation();
    AbstractNBodyEngine<T>* const m_engine;
};