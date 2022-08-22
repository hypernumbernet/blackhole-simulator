#pragma once

#include "engine/abstractnbodyengine.h"
#include "InitializerGravityCollision.h"

template <typename T>
class EngineGravityCollision : public AbstractNBodyEngine<T>, private InitializerGravityCollision<T>
{
public:
    explicit EngineGravityCollision(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , InitializerGravityCollision<T>(sim, this)
    {
        this->setNumberOfParticles();

        this->m_masses = new T[this->m_numberOfParticles];
        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 3];

        quint64 numberOfInteraction = this->m_numberOfParticles * (this->m_numberOfParticles - 1) / 2;
        this->m_distances = new T[numberOfInteraction]{0.0};

        this->setTimePerFrame(sim.timePerFrame);

        this->init();
    }

    ~EngineGravityCollision()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
    }
};
