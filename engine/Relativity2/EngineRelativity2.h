#pragma once

#include "engine/AbstractNBodyEngine.h"
#include "InitializerRelativity2.h"

template <typename T>
class EngineRelativity2 : public AbstractNBodyEngine<T>, private InitializerRelativity2<T>
{
public:
    explicit EngineRelativity2(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , InitializerRelativity2<T>(sim, this)
    {
        this->setNumberOfParticles();

        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 3];
        this->m_masses = new T[this->m_numberOfParticles];

        this->setTimePerFrame(sim.timePerFrame);

        this->init();
    }

    ~EngineRelativity2()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
    }
};
