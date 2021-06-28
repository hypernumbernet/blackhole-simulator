#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d.h"
#include "threadadmin.h"

#include <QThread>

// Gravity 3D Mass Differential N-Body Engine

class G3DMassDiffNBE : public AbstractNBodyEngine, private Initializer3D
{
public:
    explicit G3DMassDiffNBE(const bhs::SimCondition& sim)
        : AbstractNBodyEngine()
        , Initializer3D(sim, this)
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
        m_velocities = new float[m_numberOfParticles * 3];

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

    ~G3DMassDiffNBE()
    {
        delete[] m_coordinates;
        delete[] m_velocities;
        delete[] m_masses;
    }

    void setTimePerFrame(const float time)
    {
        m_timePerFrame = time;
        emit UpdateUi::it().displayTimePerFrame(time);
    }
};
