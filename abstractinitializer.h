#pragma once

#include "abstractnbodyengine.h"

class AbstractInitializer
{
public:

    AbstractInitializer(const bhs::SimCondition&, AbstractNBodyEngine* const);

protected:
    const bhs::SimCondition& m_sim;
    AbstractNBodyEngine* const m_engine;

private:
    virtual void initParticlesRandam() = 0;
    virtual void initSunEarth() = 0;
    virtual void initEarthSun() = 0;
    virtual void initEarthMoon() = 0;
    virtual void initSunEarthVenus() = 0;
    virtual void initTestSamePosition() = 0;
};
