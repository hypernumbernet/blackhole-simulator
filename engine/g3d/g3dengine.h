#pragma once

#include "engine/abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d.h"
#include "threadadmin.h"

// Gravity 3D with Mass Euler method N-Body Engine

template <typename T>
class G3DEngine : public AbstractNBodyEngine<T>, private Initializer3D<T>
{
public:
    explicit G3DEngine(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>(sim)
        , Initializer3D<T>(sim, this)
    {
        this->setNumberOfParticles();

        this->m_masses = new T[this->m_numberOfParticles];
        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 3];

        this->setTimePerFrame(sim.timePerFrame);

        this->init();
    }

    ~G3DEngine()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
    }
};
