#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d.h"
#include "quaternion.h"

using namespace bhs;

// Gravity 3D-Coordinate 3S-Velocity Mass Differential N-Body Engine

template <typename T>
class G3SVMassDiffNBE : public AbstractNBodyEngine, private Initializer3D
{
public:
    G3SVMassDiffNBE(
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

    ~G3SVMassDiffNBE()
    {
        delete[] m_coordinates;
        delete[] m_velocities;
        delete[] m_masses;
    }

    void calculateTimeProgress() const
    {
        //debug();
        for (quint64 i = 0; i < m_numberOfParticles; ++i)
        {
            quint64 j = i * 3;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame;
        }
    }

    void calculateInteraction() const
    {
        float d1, d2, d3, distance, inv, theta;
        quint64 k = 0, a, b;
        float cinv = 1.0f / SPEED_OF_LIGHT;

        // Half the circumference is assumed to be the speed of light.
        float velangle = PI * cinv;

        float vangle_half = velangle * 0.5f;
        float vangle_inv = 1.0f / velangle;
        float time_g = m_timePerFrame * GRAVITATIONAL_CONSTANT;

        for (quint64 i = 0; i < m_numberOfParticles - 1; ++i)
        {
            for (quint64 j = i + 1; j < m_numberOfParticles; ++j)
            {
                // Calculates velocity as an angle on a unit quaternion.

                a = i * 3;
                b = j * 3;

                d1 = m_coordinates[a    ] - m_coordinates[b    ];
                d2 = m_coordinates[a + 1] - m_coordinates[b + 1];
                d3 = m_coordinates[a + 2] - m_coordinates[b + 2];
                distance = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                if (distance <= 0.0f) {
                    continue;
                }
                inv = 1.0f / distance;
                theta = inv * inv * time_g;

                d1 *= inv;
                d2 *= inv;
                d3 *= inv;

                auto rota = Quaternion<float>(0.0, d1, d2, d3);
                auto rotb = Quaternion<float>(0.0, d1, d2, d3);
                rota.MakeRotation(-theta * m_masses[j] * vangle_half);
                rotb.MakeRotation(theta * m_masses[i] * vangle_half);

                auto vva = Vector3<float>(m_velocities, a);
                auto vvb = Vector3<float>(m_velocities, b);
                auto va = Quaternion<float>::Exp(vva * velangle);
                auto vb = Quaternion<float>::Exp(vvb * velangle);

                auto rotatedA = va.Rot8(rota);
                auto rotatedB = vb.Rot8(rotb);

                auto v3a = rotatedA.LnV3();
                auto v3b = rotatedB.LnV3();
                m_velocities[a    ] = v3a.x * vangle_inv;
                m_velocities[a + 1] = v3a.y * vangle_inv;
                m_velocities[a + 2] = v3a.z * vangle_inv;
                m_velocities[b    ] = v3b.x * vangle_inv;
                m_velocities[b + 1] = v3b.y * vangle_inv;
                m_velocities[b + 2] = v3b.z * vangle_inv;

                ++k;
            }
        }
    }

    void debug() const
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

    void setTimePerFrame(const float time)
    {
        m_timePerFrame = time;
        emit m_updateUi->displayTimePerFrame(time);
    }

private:

    // Physically calculated time per frame (second)
    float m_timePerFrame;
};
