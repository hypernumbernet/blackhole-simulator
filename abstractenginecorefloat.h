#pragma once

#include "abstractenginecore.h"
#include "abstractnbodyengine.h"

#include <QObject>

class AbstractEngineCoreFloat : public AbstractEngineCore
{
    Q_OBJECT
public:
    explicit AbstractEngineCoreFloat(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : m_engine(engine)
        , m_numberOfParticles(engine->numberOfParticle())
        , m_coordinates(engine->coordinates())
        , m_velocities(engine->velocities())
        , m_masses(engine->masses())
        , m_timePerFrame(engine->timePerFrame())
        , m_timeProgresStart(engine->timeProgressRanges().at(threadNumber).start)
        , m_timeProgresEnd(engine->timeProgressRanges().at(threadNumber).end)
        , m_interactionStart(engine->interactionRanges().at(threadNumber).start)
        , m_interactionEnd(engine->interactionRanges().at(threadNumber).end)
    {
    }

    bool hasRangeTimeProgress() const
    {
        return m_timeProgresEnd - m_timeProgresStart > 0;
    }

    bool hasRangeInteraction() const
    {
        return m_interactionEnd - m_interactionStart > 0;
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

    // Physically calculated time per frame (second)
    const float m_timePerFrame;

    const quint64 m_timeProgresStart;
    const quint64 m_timeProgresEnd;
    const quint64 m_interactionStart;
    const quint64 m_interactionEnd;
};
