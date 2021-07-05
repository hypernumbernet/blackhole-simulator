#pragma once

#include "abstractnbodyengine.h"

class AbstractInitializer
{
public:

    struct Correct
    {
        double m;
        double kg;
        Correct(double scale)
        {
            m = 1.0 / scale;
            kg = m * m * m;
        }
    };

    explicit AbstractInitializer(const bhs::SimCondition& sim)
        : m_sim(sim)
    {
    }
    virtual ~AbstractInitializer(){}

protected:
    const bhs::SimCondition& m_sim;

private:
    virtual void initRandamCube() = 0;
    virtual void initRandamSphere(double) = 0;
    virtual void initSunEarth() = 0;
    virtual void initEarthSun() = 0;
    virtual void initEarthMoon() = 0;
    virtual void initSunEarthVenus() = 0;
    virtual void initTestSamePosition() = 0;
};
