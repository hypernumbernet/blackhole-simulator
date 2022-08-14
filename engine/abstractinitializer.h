#pragma once

#include "engine/abstractnbodyengine.h"

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

    inline void init()
    {
        switch (m_sim.preset)
        {
        case bhs::Preset::RandomCube:
            initRandamCube();
            break;
        case bhs::Preset::RandomSphere:
            initRandamSphere(0.9);
            break;
        case bhs::Preset::RandomBall:
            initRandamSphere(0.0);
            break;
        case bhs::Preset::Custom:
            initCustom();
            break;
        case bhs::Preset::SunEarth:
            initSunEarth();
            break;
        case bhs::Preset::EarthSun:
            initEarthSun();
            break;
        case bhs::Preset::EarthMoon:
            initEarthMoon();
            break;
        case bhs::Preset::TestSamePosition:
            initTestSamePosition();
            break;
        case bhs::Preset::SunEarthVenus:
            initSunEarthVenus();
            break;
        case bhs::Preset::SunMercury:
            initSunMercury();
            break;
        }
    }

private:
    virtual void initRandamCube() = 0;
    virtual void initRandamSphere(double) = 0;
    virtual void initCustom() = 0;
    virtual void initSunEarth() = 0;
    virtual void initEarthSun() = 0;
    virtual void initEarthMoon() = 0;
    virtual void initSunEarthVenus() = 0;
    virtual void initTestSamePosition() = 0;
    virtual void initSunMercury() = 0;
};
