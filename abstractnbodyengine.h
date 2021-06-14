#pragma once

#include <math.h>

#include <QtGlobal>
#include <QDebug>

class AbstractNBodyEngine
{
public:
    AbstractNBodyEngine(quint64 numberOfParticles);
    virtual ~AbstractNBodyEngine();

    //virtual void newParticles() = 0;
    virtual float getDistance(quint64, quint64) = 0;
    virtual void timeProgress() = 0;
    virtual void calculateInteraction() = 0;
    virtual void debug() = 0;
    virtual quint64 getNumberOfParticle() = 0;
    virtual float* getCoordinates() = 0;
    virtual float getModelScale() = 0;

    static float randf(){return (float)rand()/(float)RAND_MAX;}

    static constexpr float GRAVITATIONAL_CONSTANT = 6.6743e-11f; // 2018 CODATA
    static constexpr float SPEED_OF_LIGHT = 2.99792458e+8f;

protected:
    quint64 numberOfParticles;
    float modelScale;

    // 粒子の座標
    float* coordinates;

    // 粒子の速度
    float* velocities;
};
