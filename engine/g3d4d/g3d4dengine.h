#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer/initializer3d4d.h"

// Gravity 3D-Coordinate 3S-Velocity with Mass Euler method N-Body Engine

template <typename T>
class G3D4DEngine : public AbstractNBodyEngine<T>, private Initializer3D4D<T>
{
public:
    explicit G3D4DEngine(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , Initializer3D4D<T>(sim, this)
    {
        this->setNumberOfParticles();

        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 4];
        this->m_masses = new T[this->m_numberOfParticles];

        this->setTimePerFrame(sim.timePerFrame);

        this->init();
    }

    ~G3D4DEngine()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
    }
};