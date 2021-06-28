#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d3s.h"
#include "quaternion.h"

using namespace bhs;

// Gravity 3D-Coordinate 3S-Velocity Mass Differential N-Body Engine

template <typename T>
class G3SVMassDiffNBE : public AbstractNBodyEngine, private Initializer3D3S
{
public:
    G3SVMassDiffNBE(
            UpdateUi* const updateUi,
            const bhs::SimCondition& sim)
        : AbstractNBodyEngine(updateUi)
        , Initializer3D3S(sim, this)
    {
        switch (sim.preset) {
        case bhs::Preset::RandomCube:
        case bhs::Preset::RandomSphere:
        case bhs::Preset::RandomBall:
            setNumberOfParticles(sim.numberOfParticles);
            break;
        case bhs::Preset::SunEarth:
        case bhs::Preset::EarthSun:
        case bhs::Preset::EarthMoon:
        case bhs::Preset::TestSamePosition:
            setNumberOfParticles(2);
            break;
        case bhs::Preset::SunEarthVenus:
            setNumberOfParticles(3);
            break;
        }
        m_masses = new float[m_numberOfParticles];
        m_coordinates = new double[m_numberOfParticles * 3];
        m_velocities = new float[m_numberOfParticles * 4];

        switch (sim.preset) {
        case bhs::Preset::RandomCube:
            initRandamCube();
            break;
        case bhs::Preset::RandomSphere:
            initRandamSphere(0.9f);
            break;
        case bhs::Preset::RandomBall:
            initRandamSphere(0.0f);
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
        }

        setTimePerFrame(sim.timePerFrame);
    }

    ~G3SVMassDiffNBE()
    {
        delete[] m_coordinates;
        delete[] m_velocities;
        delete[] m_masses;
    }

    void setTimePerFrame(const float time)
    {
        m_timePerFrame = time;
        emit m_updateUi->displayTimePerFrame(time);
    }
};
