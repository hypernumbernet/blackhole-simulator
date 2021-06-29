#pragma once

#include "updateui.h"

#include <math.h>

#include <QtGlobal>
#include <QDebug>
#include <QMutex>

template <typename T>
class AbstractNBodyEngine
{
public:
    static constexpr T GRAVITATIONAL_CONSTANT = 6.6743e-11; // 2018 CODATA
    static constexpr T SPEED_OF_LIGHT = 2.99792458e+8;
    static constexpr T PI = 3.141592653589793;
    static constexpr T VANGLE = PI / SPEED_OF_LIGHT;

    explicit AbstractNBodyEngine();
    virtual ~AbstractNBodyEngine();

    T* coordinates() const;
    T* velocities() const;
    T* masses() const;
    T* inversedDistances() const;

    quint64 numberOfParticle() const;
    T modelScale() const;
    T timePerFrame() const;

    void setModelScale(T);
    void setModelScaleRatio(T);
    void changeModelScale(T);
    int threadCount();
    void resultReady() const;
    QVector<bhs::IntRange> timeProgressRanges() const;
    QVector<bhs::IntRange> interactionRanges() const;

protected:
    void setNumberOfParticles(quint64);

    // Particle coordinates
    T* m_coordinates;

    // Particle velocity
    T* m_velocities;

    // Stores the mass of each particle (kg)
    T* m_masses;

    // Stores the reciprocal of the previously calculated distance.
    T* m_inversedDistances;

    // Physically calculated time per frame (second)
    T m_timePerFrame;

    quint64 m_numberOfParticles;
    T m_modelScale;
    T m_scaleCenterValue;
    QVector<bhs::IntRange> m_timeProgressRanges;
    QVector<bhs::IntRange> m_interactionRanges;
};
