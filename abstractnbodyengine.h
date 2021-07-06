#pragma once

#include "updateui.h"

#include <math.h>

#include <QtGlobal>
#include <QDebug>
#include <QMutex>
#include <QThread>

template <typename T>
class AbstractNBodyEngine
{
public:
    static constexpr T GRAVITATIONAL_CONSTANT = (T)6.6743e-11; // 2018 CODATA
    static constexpr T SPEED_OF_LIGHT = (T)2.99792458e+8;
    static constexpr T PI = (T)3.141592653589793;
    static constexpr T VANGLE = PI / SPEED_OF_LIGHT;

    explicit AbstractNBodyEngine(const bhs::SimCondition&);
    virtual ~AbstractNBodyEngine();

    T* coordinates() const;
    T* velocities() const;
    T* masses() const;

    quint64 numberOfParticle() const;

    T timePerFrame() const;
    void setTimePerFrame(const double time);

    double modelScale() const;
    void setModelScale(double);
    void setModelScaleRatio(double);
    void changeModelScale(double);

    QVector<bhs::IntRange> timeProgressRanges() const;
    QVector<bhs::IntRange> interactionRanges() const;

    T m_gravitationalConstant;
    const bhs::SimCondition& m_sim;

protected:
    void setNumberOfParticles(quint64);

    // Particle coordinates
    T* m_coordinates;

    // Particle velocity
    T* m_velocities;

    // Stores the mass of each particle (kg)
    T* m_masses;

    // Physically calculated time per frame (second)
    T m_timePerFrame;

    quint64 m_numberOfParticles;
    double m_modelScale;
    double m_scaleCenterValue;
    QVector<bhs::IntRange> m_timeProgressRanges;
    QVector<bhs::IntRange> m_interactionRanges;
};
