#pragma once

#include "engine/AbstractNBodyEngine.h"
#include "InitializerRelativity1.h"

template <typename T>
class EngineRelativity1 : public AbstractNBodyEngine<T>, private InitializerRelativity1<T>
{
public:
    explicit EngineRelativity1(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , InitializerRelativity1<T>(sim, this)
    {
        this->setNumberOfParticles();

        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 4];
        this->m_masses = new T[this->m_numberOfParticles];

        this->setTimePerFrame(sim.timePerFrame);

        this->init();
    }

    ~EngineRelativity1()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
    }
};
