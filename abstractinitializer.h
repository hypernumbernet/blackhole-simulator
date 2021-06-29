#pragma once

#include "abstractnbodyengine.h"

template <typename T>
class AbstractInitializer
{
public:

    AbstractInitializer(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : m_sim(sim)
        , m_engine(engine)
    {
    }


protected:
    const bhs::SimCondition& m_sim;
    AbstractNBodyEngine<T>* const m_engine;

private:
    virtual void initRandamCube() = 0;
    virtual void initRandamSphere(T) = 0;
    virtual void initSunEarth() = 0;
    virtual void initEarthSun() = 0;
    virtual void initEarthMoon() = 0;
    virtual void initSunEarthVenus() = 0;
    virtual void initTestSamePosition() = 0;
};
