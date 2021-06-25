#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d.h"
#include "threadadmin.h"

#include <QThread>

// Gravity 3D Mass Differential N-Body Engine

class G3DMassDiffNBE : public AbstractNBodyEngine, private Initializer3D
{
    Q_OBJECT

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

//        for (int i = 0; i < m_timeProgressRanges.size(); ++i) {
//            auto range = m_timeProgressRanges.at(i);
//            range.coordinates = m_coordinates;
//            range.velocities = m_velocities;
//            range.masses = m_masses;
//            range.timePerFrame = m_timePerFrame;
//            auto ctrl = new Controller(range);
//            m_timeProgressControllers.append(ctrl);
//        }
    }

    ~G3DMassDiffNBE()
    {
//        for (int i = 0; i < m_timeProgressRanges.size(); ++i) {
//            delete m_timeProgressRanges.at(i);
//        }

        delete[] m_coordinates;
        delete[] m_velocities;
        delete[] m_masses;
    }

    void calculateTimeProgress(int threadNumber) const override
    {
        //debug();

//        for (int i = 0; i < m_timeProgressControllers.size(); ++i) {
//            //++Controller::waitForDone;
//            m_timeProgressControllers.at(i)->calculate();
//        }
        quint64 start = m_timeProgressRanges[threadNumber].start;
        quint64 end = m_timeProgressRanges[threadNumber].end;

        for (quint64 i = start; i < end; ++i)
        {
            quint64 j = i * 3;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame;
//            if constexpr (std::is_same_v<T, double>) {
//                m_coordinates[j] += 0.0f;
//                // TODO
//            }
        }
        resultReady();
    }

    void calculateInteraction(int threadNumber) const override
    {
//        for (int i = 0; i < m_timeProgressControllers.size(); ++i) {
//            m_timeProgressControllers.at(i)->calculateInteraction();
//        }
        quint64 start = m_interactionRanges[threadNumber].start;
        quint64 end = m_interactionRanges[threadNumber].end;

        float d1, d2, d3, distance, inv, theta;
        quint64 k = 0, a, b;
        float time_g = m_timePerFrame * GRAVITATIONAL_CONSTANT;

//        if (m_numberOfParticles == 0)
//            return;

        for (quint64 i = start; i < end; ++i)
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
                Q_ASSERT(theta == theta);

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
        resultReady();
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

    void setTimePerFrame(const float time)
    {
        m_timePerFrame = time;
        emit m_updateUi->displayTimePerFrame(time);
    }

private:
    //ThreadAdmin m_threadAdmin;
    //QVector<Controller*> m_timeProgressControllers;
};
