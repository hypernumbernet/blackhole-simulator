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
        this->m_locations = new T[this->m_numberOfParticles * 4];

        quint64 numberOfInteraction = this->m_numberOfParticles * (this->m_numberOfParticles - 1) / 2;
        this->m_distances = new T[numberOfInteraction];

        this->setTimePerFrame(sim.timePerFrame);

        this->init();

        double r, dot;
        quint64 i, j, a, b;
        quint64 k = 0;
        Quaternion qa, qb;
        for (i = 0; i < this->m_numberOfParticles - 1; ++i)
        {
            a = i * 4;
            for (j = i + 1; j < this->m_numberOfParticles; ++j)
            {
                b = j * 4;
                qa.setRe(double(this->m_locations[a]));
                qa.setI1(double(this->m_locations[a + 1]));
                qa.setI2(double(this->m_locations[a + 2]));
                qa.setI3(double(this->m_locations[a + 3]));
                qb.setRe(double(this->m_locations[b]));
                qb.setI1(double(this->m_locations[b + 1]));
                qb.setI2(double(this->m_locations[b + 2]));
                qb.setI3(double(this->m_locations[b + 3]));
                dot = qa.dot(qb);
                if (dot >= 1.)
                {
                    r = std::numeric_limits<double>::min();
                }
                else if (dot < - 1.)
                {
                    r = acos(- 1.);
                }
                else
                {
                    r = acos(dot);
                }
                this->m_distances[k] = T(1. / r);
                ++k;
            }
        }
    }

    ~EngineUniverse1()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
        delete[] this->m_locations;
        delete[] this->m_distances;
    }
};
