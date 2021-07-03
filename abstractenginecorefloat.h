#pragma once

#include "abstractenginecore.h"
#include "abstractnbodyengine.h"

#include <QObject>

class AbstractEngineCoreFloat : public AbstractEngineCore
{
    Q_OBJECT
public:
    explicit AbstractEngineCoreFloat(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCore(threadNumber)
        , m_engine(engine)
        , m_numberOfParticles(engine->numberOfParticle())
        , m_coordinates(engine->coordinates())
        , m_velocities(engine->velocities())
        , m_masses(engine->masses())
        , m_inversedDistances(engine->inversedDistances())
        , m_timePerFrame(engine->timePerFrame())
        , m_start(m_engine->timeProgressRanges().at(threadNumber).start)
        , m_end(m_engine->timeProgressRanges().at(threadNumber).end)
    {
    }

    bool hasRangeTimeProgress(const int threadNum) const
    {
        auto range = m_engine->timeProgressRanges().at(threadNum);
        return range.end - range.start > 0;
    }

    bool hasRangeInteraction(const int threadNum) const
    {
        auto range = m_engine->interactionRanges().at(threadNum);
        return range.end - range.start > 0;
    }

protected:

    void debug() const
    {
        for (quint64 i = 0; i < m_numberOfParticles * 3; ++i)
        {
            qDebug() << "C: " << i << m_coordinates[i];
        }
        for (quint64 i = 0; i < m_numberOfParticles * 3; ++i)
        {
            qDebug() << "V: " << i << m_velocities[i];
        }
    }

    AbstractNBodyEngine<float>* const m_engine;
    const quint64 m_numberOfParticles;

    // Particle coordinates
    float* const m_coordinates;

    // Particle velocity
    float* const m_velocities;

    // Stores the mass of each particle (kg)
    float* const m_masses;

    // Stores the reciprocal of the previously calculated distance.
    float* const m_inversedDistances;

    // Physically calculated time per frame (second)
    const float m_timePerFrame;

    const quint64 m_start;
    const quint64 m_end;
};
