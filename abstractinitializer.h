#pragma once

#include "abstractnbodyengine.h"

class AbstractInitializer
{
public:

    explicit AbstractInitializer(const bhs::SimCondition& sim)
        : m_sim(sim)
    {
    }

protected:
    const bhs::SimCondition& m_sim;

private:
    virtual void initRandamCube() = 0;
    virtual void initRandamSphere(double) = 0;
    virtual void initSunEarth() = 0;
    virtual void initEarthSun() = 0;
    virtual void initSunEarthAu() = 0;
    virtual void initEarthMoon() = 0;
    virtual void initSunEarthVenus() = 0;
    virtual void initTestSamePosition() = 0;
};
