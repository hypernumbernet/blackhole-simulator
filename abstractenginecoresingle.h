#pragma once

#include "abstractenginecore.h"
#include "abstractnbodyengine.h"

#include <QObject>

class AbstractEngineCoreSingle : public AbstractEngineCore
{
    Q_OBJECT
public:
    explicit AbstractEngineCoreSingle(AbstractNBodyEngine<float>* const engine, QObject* parent = nullptr)
        : AbstractEngineCore(parent)
        , m_engine(engine)
        , m_numberOfParticles(engine->numberOfParticle())
        , m_coordinates((float*)engine->coordinates())
        , m_velocities(engine->velocities())
        , m_masses(engine->masses())
        , m_inversedDistances(engine->inversedDistances())
        , m_timePerFrame(engine->timePerFrame())
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
};
