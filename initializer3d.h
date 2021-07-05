#pragma once

#include "abstractinitializer.h"
#include "quaternion.h"

using namespace bhs;

template <typename T>
class Initializer3D4D;

template <typename T>
class Initializer3D : private AbstractInitializer
{
    friend Initializer3D4D<T>;

public:

    explicit Initializer3D(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : AbstractInitializer(sim)
        , m_engine(engine)
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
    AbstractNBodyEngine<T>* const m_engine;
};
