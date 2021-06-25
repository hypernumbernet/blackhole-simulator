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

    struct IntRange
    {
        quint64 start;
        quint64 end;
    };

    explicit AbstractNBodyEngine(UpdateUi*);
    virtual ~AbstractNBodyEngine();

    virtual void debug() const = 0;

    float* coordinates() const;
    float* velocities() const;
    float* masses() const;

    quint64 numberOfParticle() const;
    float modelScale() const;
    float timePerFrame() const;

    void setModelScale(float);
    void setModelScaleRatio(float);
    void changeModelScale(float);
    int threadCount();
    void resultReady() const;
    QVector<IntRange> timeProgressRanges() const;
    QVector<IntRange> interactionRanges() const;
    UpdateUi* updateUi() const;

//public slots:
//    virtual void calculateTimeProgress(int threadNumber) const = 0;
//    virtual void calculateInteraction(int threadNumber) const = 0;

protected:
    void setNumberOfParticles(quint64);

    UpdateUi* const m_updateUi;

    // Particle coordinates
    float* m_coordinates;

    // Particle velocity
    float* m_velocities;

    // Stores the mass of each particle (kg)
    float* m_masses;

    // Physically calculated time per frame (second)
    float m_timePerFrame;

    quint64 m_numberOfParticles;
    float m_modelScale;
    float m_scaleCenterValue;
    QVector<IntRange> m_timeProgressRanges;
    QVector<IntRange> m_interactionRanges;
};
