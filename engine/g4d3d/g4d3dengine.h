#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer/initializer4d3d.h"

// Gravity 3D-Coordinate 3S-Velocity with Mass Euler method N-Body Engine

template <typename T>
class G4D3DEngine : public AbstractNBodyEngine<T>, private Initializer4D3D<T>
{
public:
    explicit G4D3DEngine(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , Initializer4D3D<T>(sim, this)
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
        this->m_velocities = new T[this->m_numberOfParticles * 3];
        this->m_masses = new T[this->m_numberOfParticles];
        this->m_locations = new T[this->m_numberOfParticles * 4];

        quint64 numberOfInteraction = this->m_numberOfParticles * (this->m_numberOfParticles - 1) / 2;
        this->m_distanceInv = new T[numberOfInteraction];

        this->setTimePerFrame(sim.timePerFrame);

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

        T d1, d2, d3, r;
        quint64 a, b;
        quint64 k = 0;
        for (quint64 i = 0; i < this->m_numberOfParticles - 1; ++i)
        {
            a = i * 3;
            for (quint64 j = i + 1; j < this->m_numberOfParticles; ++j)
            {
                b = j * 3;
                d1 = this->m_coordinates[b    ] - this->m_coordinates[a    ];
                d2 = this->m_coordinates[b + 1] - this->m_coordinates[a + 1];
                d3 = this->m_coordinates[b + 2] - this->m_coordinates[a + 2];
                r = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                this->m_distanceInv[k] = T(1.0) / r;
                ++k;
            }
        }
    }

    ~G4D3DEngine()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
        delete[] this->m_locations;
        delete[] this->m_distanceInv;
    }


};
