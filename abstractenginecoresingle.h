#pragma once

#include "abstractnbodyengine.h"

#include <QObject>

class AbstractEngineCoreSingle : public QObject
{
    Q_OBJECT
public:
    explicit AbstractEngineCoreSingle(AbstractNBodyEngine<float>* const, QObject* = nullptr);

    bool hasRangeTimeProgress(int) const;
    bool hasRangeInteraction(int) const;

public slots:
    virtual void calculateTimeProgress(int threadNumber) const = 0;
    virtual void calculateInteraction(int threadNumber) const = 0;

protected:
    void resultReady() const;
    void debug() const;

    AbstractNBodyEngine<float>* const m_engine;
    quint64 m_numberOfParticles;

    // Particle coordinates
    float* const m_coordinates;

    // Particle velocity
    float* const m_velocities;

    // Stores the mass of each particle (kg)
    float* const m_masses;

    // Stores the reciprocal of the previously calculated distance.
    float* const m_inversedDistances;

    // Physically calculated time per frame (second)
    float m_timePerFrame;
};
