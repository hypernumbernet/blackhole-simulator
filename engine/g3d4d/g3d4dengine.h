#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer/initializer3d4d.h"

// Gravity 3D-Coordinate 3S-Velocity with Mass Euler method N-Body Engine

template <typename T>
class G3D4DEngine : public AbstractNBodyEngine<T>, private Initializer3D4D<T>
{
public:
    explicit G3D4DEngine(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , Initializer3D4D<T>(sim, this)
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

        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 4];
        this->m_masses = new T[this->m_numberOfParticles];
//        this->m_curvature = new T[this->m_numberOfParticles];
//        for (quint64 i = 0; i < this->m_numberOfParticles; ++i)
//            this->m_curvature[i] = 1.0;

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

    ~G3D4DEngine()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
//        delete[] this->m_curvature;
    }
};
