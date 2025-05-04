#pragma once

#include "engine/AbstractNBodyEngine.h"
#include "InitializerRelativity4.h"

template <typename T>
class EngineRelativity4 : public AbstractNBodyEngine<T>, private InitializerRelativity4<T>
{
public:
    explicit EngineRelativity4(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , InitializerRelativity4<T>(sim, this)
    {
        this->setNumberOfParticles();

        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 4];
        this->m_masses = new T[this->m_numberOfParticles];

        this->setTimePerFrame(sim.timePerFrame);

        this->init();
    }

    ~EngineRelativity4()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
    }
};
