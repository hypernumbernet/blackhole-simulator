#pragma once

#include "updateui.h"
#include "hnn/quaternion.h"

#include <QtGlobal>
#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QVector4D>
#include <QGenericMatrix>

using namespace hnn;

template <typename T>
class AbstractNBodyEngine
{
public:
    static constexpr T GRAVITATIONAL_CONSTANT = T(6.6743e-11); // 2018 CODATA
    static constexpr T SPEED_OF_LIGHT = T(2.99792458e+8);
    static constexpr T PI = T(3.141592653589793);
    static constexpr T VANGLE = T(0.5) * PI / SPEED_OF_LIGHT;
    static constexpr T BOUNDARY_TO_INVALIDATE = T(0.005);

    explicit AbstractNBodyEngine(const bhs::SimCondition&);
    virtual ~AbstractNBodyEngine();

    T* coordinates() const;
    T* velocities() const;
    T* masses() const;
    T* distanceInv() const;
    T* locations() const;

    const bhs::SimCondition& sim() const;
    quint64 numberOfParticle() const;
    T timePerFrame() const;
    void setTimePerFrame(double);
    T gravitationalConstant() const;
    void setGravitationalConstant(T);

    double modelScale() const;
    void setModelScale(double);
    void setModelScaleRatio(double);
    void changeModelScale(double);

    QVector<bhs::IntRange> timeProgressRanges() const;
    QVector<bhs::IntRange> interactionRanges() const;

    T velocityToAngle(T) const;
    void angleToVelocity(Vector3<T>&) const;
    QGenericMatrix<4, 4, T> LorentzTransformation(T, T, T) const;
    QGenericMatrix<4, 4, T> LorentzTransformation(const Vector3<T>&) const;

protected:
    void setNumberOfParticles(quint64);
    void setTimeProgressRanges(int threadCount);
    void setInteractionRanges(int threadCount);

    const bhs::SimCondition& m_sim;

    // Particle coordinates
    T* m_coordinates;

    // Particle velocity
    T* m_velocities;

    // Stores the mass of each particle (kg)
    T* m_masses;

    // Physically calculated time per frame (second)
    T m_timePerFrame;

    T* m_distanceInv;

    T* m_locations;

    quint64 m_numberOfParticles;
    double m_modelScale;
    double m_scaleCenterValue;
    T m_gravitationalConstant;
    QVector<bhs::IntRange> m_timeProgressRanges;
    QVector<bhs::IntRange> m_interactionRanges;
};
