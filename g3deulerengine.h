#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d.h"
#include "threadadmin.h"

// Gravity 3D with Mass Euler method N-Body Engine

template <typename T>
class G3DEulerEngine : public AbstractNBodyEngine<T>, private Initializer3D<T>
{
public:
    explicit G3DEulerEngine(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , Initializer3D<T>(sim, this)
    {
        switch (sim.preset)
        {
        case bhs::Preset::RandomCube:
        case bhs::Preset::RandomSphere:
        case bhs::Preset::RandomBall:
            this->setNumberOfParticles(sim.numberOfParticles);
            break;
        case bhs::Preset::SunEarth:
        case bhs::Preset::EarthSun:
        case bhs::Preset::EarthMoon:
        case bhs::Preset::TestSamePosition:
            this->setNumberOfParticles(2);
            break;
        case bhs::Preset::SunEarthVenus:
            this->setNumberOfParticles(3);
            break;
        }
        this->m_masses = new T[this->m_numberOfParticles];
        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 3];

        switch (sim.preset)
        {
        case bhs::Preset::RandomCube:
            this->initRandamCube();
            break;
        case bhs::Preset::RandomSphere:
            this->initRandamSphere(0.9);
            break;
        case bhs::Preset::RandomBall:
            this->initRandamSphere(0.0);
            break;
        case bhs::Preset::SunEarth:
            this->initSunEarth();
            break;
        case bhs::Preset::EarthSun:
            this->initEarthSun();
            break;
        case bhs::Preset::EarthMoon:
            this->initEarthMoon();
            break;
        case bhs::Preset::TestSamePosition:
            this->initTestSamePosition();
            break;
        case bhs::Preset::SunEarthVenus:
            this->initSunEarthVenus();
            break;
        }

        this->setTimePerFrame(sim.timePerFrame);
    }

    ~G3DEulerEngine()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
    }
};
