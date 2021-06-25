#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d.h"

// Gravity 3D Mass Integral N-Body Engine

template <typename T>
class G3DMassIntegralNBE : public AbstractNBodyEngine, private Initializer3D
{
    struct Distance {
        float invR;
        float unitX;
        float unitY;
        float unitZ;
    };

public:
    G3DMassIntegralNBE(
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

        quint64 numberOfInteractions = sim.numberOfParticles * (sim.numberOfParticles - 1) / 2;
        m_inversedDistances = new float[numberOfInteractions];

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
        calculateDistances();
    }

    ~G3DMassIntegralNBE()
    {
        delete[] m_coordinates;
        delete[] m_velocities;
        delete[] m_masses;
        delete[] m_inversedDistances;
    }

//    void calculateTimeProgress(int ) const override
//    {
//        //debug();
//        for (quint64 i = 0; i < m_numberOfParticles; ++i)
//        {
//            quint64 j = i * 3;
//            m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
//            m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
//            m_coordinates[j] += m_velocities[j] * m_timePerFrame;
//        }
//    }

//    void calculateInteraction(int) const override
//    {
//        // Perform integral calculation of universal gravitation.
//        // The memory cost is high because the distance data calculated last time is saved and used.
//        // However, if the distance variation is large, it may be more accurate to calculate by this method.
//        // The celestial scale vibrates violently at float resolution.

//        float inv, force;
//        Distance d;
//        quint64 k = 0, a, b;

//        if (m_numberOfParticles == 0)
//            return;

//        for (quint64 i = 0; i < m_numberOfParticles - 1; ++i)
//        {
//            for (quint64 j = i + 1; j < m_numberOfParticles; ++j)
//            {
//                if (!calculateDistance(d, i, j))
//                    continue;

//                inv = m_inversedDistances[k];
//                m_inversedDistances[k] = d.invR;
//                inv -= d.invR;

//                if (inv == 0.0f)
//                {
//                    continue;
//                }

//                // Time is not taken into account.
//                force = abs(inv * GRAVITATIONAL_CONSTANT);

//                d.unitX *= force;
//                d.unitY *= force;
//                d.unitZ *= force;

//                a = i * 3;
//                b = j * 3;
//                m_velocities[a] -= d.unitX * m_masses[j];
//                m_velocities[a + 1] -= d.unitY * m_masses[j];
//                m_velocities[a + 2] -= d.unitZ * m_masses[j];
//                m_velocities[b] += d.unitX * m_masses[i];
//                m_velocities[b + 1] += d.unitY * m_masses[i];
//                m_velocities[b + 2] += d.unitZ * m_masses[i];

//                ++k;
//            }
//        }
//    }

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

    void setTimePerFrame(const float time)
    {
        m_timePerFrame = time;
        emit m_updateUi->displayTimePerFrame(time);
    }

    bool calculateDistance(Distance& result, const quint64 a, const quint64 b) const
    {
        quint64 ai = a * 3;
        quint64 bi = b * 3;
        float dx = m_coordinates[ai] - m_coordinates[bi]; ++ai; ++bi;
        float dy = m_coordinates[ai] - m_coordinates[bi]; ++ai; ++bi;
        float dz = m_coordinates[ai] - m_coordinates[bi];
        float r = sqrt(dx * dx + dy * dy + dz * dz);
        if (r == 0.0f)
        {
            return false;
        }
        r = 1.0f / r;
        result.invR = r;
        result.unitX = dx * r;
        result.unitY = dy * r;
        result.unitZ = dz * r;
        return true;
    }

private:

    void calculateDistances()
    {
        quint64 k = 0;
        Distance d;

        if (m_numberOfParticles == 0)
            return;

        for (quint64 i = 0; i < m_numberOfParticles - 1; ++i)
        {
            for (quint64 j = i + 1; j < m_numberOfParticles; ++j)
            {
                if (!calculateDistance(d, i, j))
                {
                    continue;
                }
                m_inversedDistances[k] = d.invR;

                ++k;
            }
        }
    }

    // Stores the reciprocal of the previously calculated distance.
    float* m_inversedDistances;
};
