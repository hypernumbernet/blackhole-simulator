#pragma once

#include "abstractnbodyengine.h"

#include <QObject>

class AbstractEngineCoreDouble : public QObject
{
    Q_OBJECT
public:
    explicit AbstractEngineCoreDouble(AbstractNBodyEngine<double>* const, QObject* = nullptr);

    bool hasRangeTimeProgress(int) const;
    bool hasRangeInteraction(int) const;

public slots:
    virtual void calculateTimeProgress(int threadNumber) const = 0;
    virtual void calculateInteraction(int threadNumber) const = 0;

protected:
    void resultReady() const;
    void debug() const;

    AbstractNBodyEngine<double>* const m_engine;
    quint64 m_numberOfParticles;

    // Particle coordinates
    double* const m_coordinates;

    // Particle velocity
    double* const m_velocities;

    // Stores the mass of each particle (kg)
    double* const m_masses;

    // Stores the reciprocal of the previously calculated distance.
    double* const m_inversedDistances;

    // Physically calculated time per frame (second)
    double m_timePerFrame;
};
