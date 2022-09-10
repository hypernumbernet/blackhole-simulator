#pragma once

#include "engine/abstractnbodyengine.h"
#include "initializer3d4dr1.h"

template <typename T>
class Engine3D4DR1 : public AbstractNBodyEngine<T>, private Initializer3D4DR1<T>
{
public:
    explicit Engine3D4DR1(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , Initializer3D4DR1<T>(sim, this)
    {
        this->setNumberOfParticles();

        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 4];
        this->m_masses = new T[this->m_numberOfParticles];

        this->setTimePerFrame(sim.timePerFrame);

        this->init();
    }

    ~Engine3D4DR1()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
    }
};
