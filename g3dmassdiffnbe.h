#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d.h"

// Gravity 3D Mass Differential N-Body Engine

template <typename T>
class G3DMassDiffNBE : public AbstractNBodyEngine, private Initializer3D
{
public:
    G3DMassDiffNBE(
            UpdateUi* const updateUi,
            const bhs::SimCondition& sim)
        : AbstractNBodyEngine(updateUi)
        , Initializer3D(sim, this)
    {
        switch (sim.preset) {
        case bhs::Preset::Random:
            setNumberOfParticles(sim.numberOfParticles);
            break;
        case bhs::Preset::SunEarth:
        case bhs::Preset::EarthSun:
        case bhs::Preset::EarthMoon:
        case bhs::Preset::TestSamePosition:
            setNumberOfParticles(2);
            break;
        case bhs::Preset::SunEarthVenus:
            setNumberOfParticles(3);
            break;
        }
        m_masses = new float[m_numberOfParticles];
        m_coordinates = new float[m_numberOfParticles * 3];
        m_velocities = new float[m_numberOfParticles * 3];

        switch (sim.preset) {
        case bhs::Preset::Random:
            initParticlesRandam();
            break;
        case bhs::Preset::SunEarth:
            initSunEarth();
            break;
        case bhs::Preset::EarthSun:
            initEarthSun();
            break;
        case bhs::Preset::EarthMoon:
            initEarthMoon();
            break;
        case bhs::Preset::TestSamePosition:
            initTestSamePosition();
            break;
        case bhs::Preset::SunEarthVenus:
            initSunEarthVenus();
            break;
        }

        setTimePerFrame(sim.timePerFrame);
    }

    ~G3DMassDiffNBE()
    {
        delete[] m_coordinates;
        delete[] m_velocities;
        delete[] m_masses;
    }

    void calculateTimeProgress() const override
    {
        //debug();
        for (quint64 i = 0; i < m_numberOfParticles; ++i)
        {
            quint64 j = i * 3;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame;
            if constexpr (std::is_same_v<T, double>) {
                m_coordinates[j] += 0.0f;
                // TODO
            }
        }
    }

    void calculateInteraction() const override
    {
        float d1, d2, d3, distance, inv, theta;
        quint64 k = 0, a, b;
        float time_g = m_timePerFrame * GRAVITATIONAL_CONSTANT;

        for (quint64 i = 0; i < m_numberOfParticles - 1; ++i)
        {
            for (quint64 j = i + 1; j < m_numberOfParticles; ++j)
            {
                // Perform differential calculation of universal gravitation.
                a = i * 3;
                b = j * 3;
                d1 = m_coordinates[a] - m_coordinates[b];
                d2 = m_coordinates[a + 1] - m_coordinates[b + 1];
                d3 = m_coordinates[a + 2] - m_coordinates[b + 2];
                distance = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                if (distance <= 0.0f) {
                    continue;
                }
                inv = 1.0f / distance;
                theta = inv * inv * time_g;

                d1 *= inv * theta;
                d2 *= inv * theta;
                d3 *= inv * theta;

                m_velocities[a] -= d1 * m_masses[j];
                m_velocities[a + 1] -= d2 * m_masses[j];
                m_velocities[a + 2] -= d3 * m_masses[j];
                m_velocities[b] += d1 * m_masses[i];
                m_velocities[b + 1] += d2 * m_masses[i];
                m_velocities[b + 2] += d3 * m_masses[i];

                ++k;
            }
        }
    }

    void debug() const override
    {
        for (quint64 i = 0; i < m_numberOfParticles * 3; ++i)
        {
            qDebug() << "xyz:" << i << m_coordinates[i];
        }
        for (quint64 i = 0; i < m_numberOfParticles * 3; ++i)
        {
            qDebug() << "v:" << i << m_velocities[i];
        }
    }

    void setTimePerFrame(const T time)
    {
        m_timePerFrame = time;
        emit m_updateUi->displayTimePerFrame(time);
    }

private:

    // Physically calculated time per frame (second)
    T m_timePerFrame;
};
