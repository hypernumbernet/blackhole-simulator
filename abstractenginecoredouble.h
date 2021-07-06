#pragma once

#include "abstractenginecore.h"
#include "abstractnbodyengine.h"

#include <QObject>

class AbstractEngineCoreDouble : public AbstractEngineCore
{
    Q_OBJECT
public:
    explicit AbstractEngineCoreDouble(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : m_engine(engine)
        , m_numberOfParticles(engine->numberOfParticle())
        , m_coordinates(engine->coordinates())
        , m_velocities(engine->velocities())
        , m_masses(engine->masses())
        , m_inversedDistances(engine->inversedDistances())
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

    AbstractNBodyEngine<double>* const m_engine;
    const quint64 m_numberOfParticles;

    // Particle coordinates
    double* const m_coordinates;

    // Particle velocity
    double* const m_velocities;

    // Stores the mass of each particle (kg)
    double* const m_masses;

    // Stores the reciprocal of the previously calculated distance.
    double* const m_inversedDistances;

    // Physically calculated time per frame (second)
    const double m_timePerFrame;

    const quint64 m_timeProgresStart;
    const quint64 m_timeProgresEnd;
    const quint64 m_interactionStart;
    const quint64 m_interactionEnd;
};
