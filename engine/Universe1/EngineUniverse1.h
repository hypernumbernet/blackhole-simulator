#pragma once

#include "engine/AbstractNBodyEngine.h"
#include "InitializerUniverse1.h"

// Gravity 3D-Coordinate 3S-Velocity with Mass Euler method N-Body Engine

template <typename T>
class EngineUniverse1 : public AbstractNBodyEngine<T>, private InitializerUniverse1<T>
{
public:
    explicit EngineUniverse1(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , InitializerUniverse1<T>(sim, this)
    {
        this->setNumberOfParticles();

        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 4];
        this->m_masses = new T[this->m_numberOfParticles];

        this->setTimePerFrame(sim.timePerFrame);

        this->init();
    }

    ~EngineUniverse1()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
    }
};
