#pragma once

#include "updateui.h"

#include <math.h>

#include <QtGlobal>
#include <QDebug>

class AbstractNBodyEngine
{
public:
    AbstractNBodyEngine(UpdateUi*);
    virtual ~AbstractNBodyEngine();

    virtual void calculateTimeProgress() const = 0;
    virtual void calculateInteraction() const = 0;
    virtual void debug() const = 0;

    virtual float* coordinates() const; // TODO virtual ???
    virtual quint64 numberOfParticle() const;
    virtual float modelScale() const;

    void setModelScale(float);
    void setModelScaleRatio(float);
    void changeModelScale(float);
    float* masses() const;
    float* velocities() const;

    static constexpr float GRAVITATIONAL_CONSTANT = 6.6743e-11f; // 2018 CODATA
    static constexpr float SPEED_OF_LIGHT = 2.99792458e+8f;

protected:
    static float randf() {return (float)rand() / (float)RAND_MAX;}

    void setNumberOfParticles(quint64);

    UpdateUi* const m_updateUi;
    float* m_coordinates; // Particle coordinates
    float* m_velocities; // Particle velocity

    // Stores the mass of each particle (kg)
    float* m_masses;

    quint64 m_numberOfParticles;
    float m_modelScale;
    float m_scaleCenterValue;
};
