#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d.h"

// Gravity 3D Mass Integral N-Body Engine

template <typename T>
class G3DMassIntegralNBE : public AbstractNBodyEngine<T>, private Initializer3D<T>
{
public:

    struct Distance
    {
        T invR;
        T unitX;
        T unitY;
        T unitZ;
    };

    G3DMassIntegralNBE(const bhs::SimCondition& sim)
        : AbstractNBodyEngine<T>()
        , Initializer3D<T>(sim, this)
    {
        switch (sim.preset)
        {
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

        quint64 numberOfInteractions = sim.numberOfParticles * (sim.numberOfParticles - 1) / 2;
        this->m_inversedDistances = new T[numberOfInteractions];

        switch (sim.preset)
        {
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
        calculateDistances();
    }

    ~G3DMassIntegralNBE()
    {
        delete[] this->m_coordinates;
        delete[] this->m_velocities;
        delete[] this->m_masses;
        delete[] this->m_inversedDistances;
    }

    void setTimePerFrame(const T time)
    {
        this->m_timePerFrame = time;
        emit UpdateUi::it().displayTimePerFrame(time);
    }

    static inline bool calculateDistance(
            Distance& result,
            const T* const coord,
            const quint64 a,
            const quint64 b)
    {
        quint64 ai = a * 3;
        quint64 bi = b * 3;
        T dx = coord[ai] - coord[bi]; ++ai; ++bi;
        T dy = coord[ai] - coord[bi]; ++ai; ++bi;
        T dz = coord[ai] - coord[bi];
        T r = sqrt(dx * dx + dy * dy + dz * dz);
        if (r == 0.0)
            return false;

        r = 1.0 / r;
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

        if (this->m_numberOfParticles == 0)
            return;

        for (quint64 i = 0; i < this->m_numberOfParticles - 1; ++i)
        {
            for (quint64 j = i + 1; j < this->m_numberOfParticles; ++j)
            {
                if (!calculateDistance(d, this->m_coordinates, i, j))
                    continue;

                this->m_inversedDistances[k] = d.invR;

                ++k;
            }
        }
    }
};
