#pragma once

#include "engine/AbstractNBodyEngine.h"
#include "initializer4d3d.h"

// Gravity 3D-Coordinate 3S-Velocity with Mass Euler method N-Body Engine

template <typename T>
class EngineUniverse2 : public AbstractNBodyEngine<T>, private InitializerUniverse2<T>
{
public:
    explicit EngineUniverse2(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , InitializerUniverse2<T>(sim, this)
    {
        this->setNumberOfParticles();

        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 3];
        this->m_masses = new T[this->m_numberOfParticles];
        this->m_locations = new T[this->m_numberOfParticles * 4];

        quint64 numberOfInteraction = this->m_numberOfParticles * (this->m_numberOfParticles - 1) / 2;
        this->m_distances = new T[numberOfInteraction];

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
                this->m_distances[k] = T(1.0) / r;
                ++k;
            }
        }
    }

    ~EngineUniverse2()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
        delete[] this->m_locations;
        delete[] this->m_distances;
    }

};
