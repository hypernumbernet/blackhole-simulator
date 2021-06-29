#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d.h"
#include "threadadmin.h"

#include <QThread>

// Gravity 3D Mass Differential N-Body Engine

template <typename T>
class G3DMassDiffNBE : public AbstractNBodyEngine<T>, private Initializer3D<T>
{
public:
    explicit G3DMassDiffNBE(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>()
        , Initializer3D<T>(sim, this)
    {
        switch (sim.preset) {
        case bhs::Preset::RandomCube:
        case bhs::Preset::RandomSphere:
        case bhs::Preset::RandomBall:
            this->setNumberOfParticles(sim.numberOfParticles);
            break;
        case bhs::Preset::SunEarth:
        case bhs::Preset::EarthSun:
        case bhs::Preset::EarthMoon:
        case bhs::Preset::TestSamePosition:
            this->setNumberOfParticles(2);
            break;
        case bhs::Preset::SunEarthVenus:
            this->setNumberOfParticles(3);
            break;
        }
        this->m_masses = new T[this->m_numberOfParticles];
        this->m_coordinates = new T[this->m_numberOfParticles * 3];
        this->m_velocities = new T[this->m_numberOfParticles * 3];

        switch (sim.preset) {
        case bhs::Preset::RandomCube:
            this->initRandamCube();
            break;
        case bhs::Preset::RandomSphere:
            this->initRandamSphere(0.9f);
            break;
        case bhs::Preset::RandomBall:
            this->initRandamSphere(0.0f);
            break;
        case bhs::Preset::SunEarth:
            this->initSunEarth();
            break;
        case bhs::Preset::EarthSun:
            this->initEarthSun();
            break;
        case bhs::Preset::EarthMoon:
            this->initEarthMoon();
            break;
        case bhs::Preset::TestSamePosition:
            this->initTestSamePosition();
            break;
        case bhs::Preset::SunEarthVenus:
            this->initSunEarthVenus();
            break;
        }

        setTimePerFrame(sim.timePerFrame);
    }

    ~G3DMassDiffNBE()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
    }

    void setTimePerFrame(const float time)
    {
        this->m_timePerFrame = time;
        emit UpdateUi::it().displayTimePerFrame(time);
    }

    void calculateTimeProgress(const int threadNumber) const
    {
        quint64 start = this->m_timeProgressRanges.at(threadNumber).start;
        quint64 end = this->m_timeProgressRanges.at(threadNumber).end;

        for (quint64 i = start; i < end; ++i)
        {
            quint64 j = i * 3;
            this->m_coordinates[j] += this->m_velocities[j] * this->m_timePerFrame; ++j;
            this->m_coordinates[j] += this->m_velocities[j] * this->m_timePerFrame; ++j;
            this->m_coordinates[j] += this->m_velocities[j] * this->m_timePerFrame;
        }

        resultReady();
    }

    void calculateInteraction(const int threadNumber) const
    {
        quint64 start = this->m_timeProgressRanges.at(threadNumber).start;
        quint64 end = this->m_timeProgressRanges.at(threadNumber).end;

        float d1, d2, d3, distance, inv, theta;
        quint64 k = 0, a, b;
        float time_g = this->m_timePerFrame * AbstractNBodyEngine<float>::GRAVITATIONAL_CONSTANT;

        float* vels = new float[this->m_numberOfParticles * 3];
        for (quint64 i = 0; i < this->m_numberOfParticles * 3; ++i) {
            vels[i] = 0.0f;
        }

        for (quint64 i = start; i < end; ++i)
        {
            for (quint64 j = i + 1; j < this->m_numberOfParticles; ++j)
            {
                // Perform differential calculation of universal gravitation.
                a = i * 3;
                b = j * 3;
                d1 = this->m_coordinates[a] - this->m_coordinates[b];
                d2 = this->m_coordinates[a + 1] - this->m_coordinates[b + 1];
                d3 = this->m_coordinates[a + 2] - this->m_coordinates[b + 2];
                distance = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                if (distance <= 0.0f) {
                    continue;
                }
                inv = 1.0f / distance;
                theta = inv * inv * time_g;
                //Q_ASSERT(theta == theta);

                d1 *= inv * theta;
                d2 *= inv * theta;
                d3 *= inv * theta;

                vels[a] -= d1 * this->m_masses[j];
                vels[a + 1] -= d2 * this->m_masses[j];
                vels[a + 2] -= d3 * this->m_masses[j];
                vels[b] += d1 * this->m_masses[i];
                vels[b + 1] += d2 * this->m_masses[i];
                vels[b + 2] += d3 * this->m_masses[i];

                ++k;
            }
        }
        bhs::interactionMutex.lock();
        for (quint64 i = 0; i < this->m_numberOfParticles * 3; ++i) {
            this->m_velocities[i] += vels[i];
        }
        bhs::interactionMutex.unlock();

        delete[] vels;

        resultReady();
    }
};
