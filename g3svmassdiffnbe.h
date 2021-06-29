#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d3s.h"
#include "quaternion.h"

using namespace bhs;

// Gravity 3D-Coordinate 3S-Velocity Mass Differential N-Body Engine

template <typename T>
class G3SVMassDiffNBE : public AbstractNBodyEngine<T>, private Initializer3D3S<T>
{
public:
    G3SVMassDiffNBE(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>()
        , Initializer3D3S<T>(sim, this)
    {
        switch (sim.preset) {
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
        this->m_velocities = new T[this->m_numberOfParticles * 4];

        switch (sim.preset) {
        case bhs::Preset::RandomCube:
            this->initRandamCube();
            break;
        case bhs::Preset::RandomSphere:
            this->initRandamSphere(0.9f);
            break;
        case bhs::Preset::RandomBall:
            this->initRandamSphere(0.0f);
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

        setTimePerFrame(sim.timePerFrame);
    }

    ~G3SVMassDiffNBE()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
    }

    void setTimePerFrame(const float time)
    {
        this->m_timePerFrame = time;
        emit UpdateUi::it().displayTimePerFrame(time);
    }
};
