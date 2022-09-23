#pragma once

#include "bhs.h"

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

    void init()
    {
        switch (m_sim.preset)
        {
        case bhs::Preset::RandomCube:
            initRandomCube();
            break;
        case bhs::Preset::RandomSphere:
            initRandomSphere(0.9);
            break;
        case bhs::Preset::RandomBall:
            initRandomSphere(0.0);
            break;
        case bhs::Preset::Custom:
            initCustom();
            break;
        }
    }

private:
    virtual void initRandomCube() = 0;
    virtual void initRandomSphere(double) = 0;
    virtual void initCustom() = 0;
};
