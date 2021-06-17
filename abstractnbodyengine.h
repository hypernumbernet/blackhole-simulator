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

    virtual float calculateDistance(quint64, quint64) const = 0;
    virtual void timeProgress() const = 0;
    virtual void calculateInteraction() const = 0;
    virtual void debug() const = 0;

    virtual quint64 numberOfParticle() const = 0;
    void setModelScale(float);
    virtual float modelScale() const = 0;
    virtual float* coordinates() const = 0;

    static constexpr float GRAVITATIONAL_CONSTANT = 6.6743e-11f; // 2018 CODATA
    static constexpr float SPEED_OF_LIGHT = 2.99792458e+8f;

protected:
    static float randf() {return (float)rand() / (float)RAND_MAX;}

    void setNumberOfParticles(quint64);
    quint64 m_numberOfParticles;

    void changeModelScale(float);
    float m_modelScale;

    // 粒子の座標
    float* m_coordinates;

    // 粒子の速度
    float* m_velocities;

    UpdateUi* const m_updateUi;
};
