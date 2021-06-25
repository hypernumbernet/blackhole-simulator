#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d.h"

// Gravity 3D Mass Integral N-Body Engine

template <typename T>
class G3DMassIntegralNBE : public AbstractNBodyEngine, private Initializer3D
{
public:

    struct Distance {
        T invR;
        T unitX;
        T unitY;
        T unitZ;
    };

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

    void setTimePerFrame(const float time)
    {
        m_timePerFrame = time;
        emit m_updateUi->displayTimePerFrame(time);
    }

    static inline bool calculateDistance(
            Distance& result,
            const T* const coord,
            const quint64 a,
            const quint64 b)
    {
        quint64 ai = a * 3;
        quint64 bi = b * 3;
        float dx = coord[ai] - coord[bi]; ++ai; ++bi;
        float dy = coord[ai] - coord[bi]; ++ai; ++bi;
        float dz = coord[ai] - coord[bi];
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
                if (!calculateDistance(d, m_coordinates, i, j))
                {
                    continue;
                }
                m_inversedDistances[k] = d.invR;

                ++k;
            }
        }
    }
};
