#pragma once

#include "engine/abstractnbodyengine.h"
#include "InitializerGravityElasticity.h"

template <typename T>
class EngineGravityElasticity : public AbstractNBodyEngine<T>, private InitializerGravityElasticity<T>
{
public:
    explicit EngineGravityElasticity(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , InitializerGravityElasticity<T>(sim, this)
    {
        this->setNumberOfParticles();

        this->m_masses = new T[this->m_numberOfParticles];
        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 3];

        this->setTimePerFrame(sim.timePerFrame);

        this->init();
    }

    ~EngineGravityElasticity()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
    }
};
