#pragma once

#include "abstractnbodyengine.h"

#include <QObject>

class AbstractEngineCore : public QObject
{
    Q_OBJECT
public:
    explicit AbstractEngineCore(AbstractNBodyEngine* const, QObject* = nullptr);

public slots:
    virtual void calculateTimeProgress(int threadNumber) const = 0;
    virtual void calculateInteraction(int threadNumber) const = 0;

protected:
    void resultReady() const;

    AbstractNBodyEngine* const m_engine;
    quint64 m_numberOfParticles;

    // Particle coordinates
    float* m_coordinates;

    // Particle velocity
    float* m_velocities;

    // Stores the mass of each particle (kg)
    float* m_masses;

    // Physically calculated time per frame (second)
    float m_timePerFrame;
};
