#pragma once

#include "abstractnbodyengine.h"

class AbstractInitializer
{
public:

    AbstractInitializer(const bhs::SimCondition&);
    //virtual ~AbstractInitializer();

protected:
    static float randf() {return (float)rand() / (float)RAND_MAX;} // TODO  to one place
    float m_massAverage;

private:
    virtual void initParticlesRandam(AbstractNBodyEngine* const) = 0;
    virtual void initSunEarth(AbstractNBodyEngine* const) = 0;
    virtual void initEarthSun(AbstractNBodyEngine* const) = 0;
    virtual void initEarthMoon(AbstractNBodyEngine* const) = 0;
    virtual void initSunEarthVenus(AbstractNBodyEngine* const) = 0;
    virtual void initTestSamePosition(AbstractNBodyEngine* const) = 0;
};
