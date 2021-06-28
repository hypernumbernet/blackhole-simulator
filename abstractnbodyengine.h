#pragma once

#include "updateui.h"

#include <math.h>

#include <QtGlobal>
#include <QDebug>
#include <QMutex>

class AbstractNBodyEngine
{
public:
    static constexpr float GRAVITATIONAL_CONSTANT = 6.6743e-11f; // 2018 CODATA
    static constexpr float SPEED_OF_LIGHT = 2.99792458e+8f;
    static constexpr float PI = 3.141592653589793f;
    static constexpr float VANGLE = PI / SPEED_OF_LIGHT;

    struct IntRange
    {
        quint64 start;
        quint64 end;
    };

    explicit AbstractNBodyEngine();
    virtual ~AbstractNBodyEngine();

    double* coordinates() const;
    float* velocities() const;
    float* masses() const;
    float* inversedDistances() const;

    quint64 numberOfParticle() const;
    float modelScale() const;
    float timePerFrame() const;

    void setModelScale(float);
    void setModelScaleRatio(double);
    void changeModelScale(float);
    int threadCount();
    void resultReady() const;
    QVector<IntRange> timeProgressRanges() const;
    QVector<IntRange> interactionRanges() const;

protected:
    void setNumberOfParticles(quint64);

    // Particle coordinates
    double* m_coordinates;

    // Particle velocity
    float* m_velocities;

    // Stores the mass of each particle (kg)
    float* m_masses;

    // Stores the reciprocal of the previously calculated distance.
    float* m_inversedDistances;

    // Physically calculated time per frame (second)
    float m_timePerFrame;

    quint64 m_numberOfParticles;
    double m_modelScale;
    double m_scaleCenterValue;
    QVector<IntRange> m_timeProgressRanges;
    QVector<IntRange> m_interactionRanges;
};
