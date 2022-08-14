#pragma once

#include "engine/abstractnbodyengine.h"
#include "updateui.h"
#include "initializer4d3d.h"

// Gravity 3D-Coordinate 3S-Velocity with Mass Euler method N-Body Engine

template <typename T>
class EngineG4D3D : public AbstractNBodyEngine<T>, private Initializer4D3D<T>
{
public:
    explicit EngineG4D3D(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , Initializer4D3D<T>(sim, this)
    {
        this->setNumberOfParticles();

        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 3];
        this->m_masses = new T[this->m_numberOfParticles];
        this->m_locations = new T[this->m_numberOfParticles * 4];

        quint64 numberOfInteraction = this->m_numberOfParticles * (this->m_numberOfParticles - 1) / 2;
        this->m_distanceInv = new T[numberOfInteraction];

        this->setTimePerFrame(sim.timePerFrame);

        this->init();

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

    ~EngineG4D3D()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
        delete[] this->m_locations;
        delete[] this->m_distanceInv;
    }

};
