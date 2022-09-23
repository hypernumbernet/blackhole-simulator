#pragma once

#include "engine/AbstractNBodyEngine.h"
#include "InitializerRelativity3.h"

template <typename T>
class EngineRelativity3 : public AbstractNBodyEngine<T>, private InitializerRelativity3<T>
{
public:
    explicit EngineRelativity3(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , InitializerRelativity3<T>(sim, this)
    {
        this->setNumberOfParticles();

        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 4];
        this->m_masses = new T[this->m_numberOfParticles];

        this->setTimePerFrame(sim.timePerFrame);

        this->init();
    }

    ~EngineRelativity3()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
    }
};
