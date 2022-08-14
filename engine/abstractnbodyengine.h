#pragma once

#include <QtGlobal>
#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QVector4D>
#include <QGenericMatrix>

#include "hnn/quaternion.h"
#include "updateui.h"

using namespace hnn;

template <typename T>
class AbstractNBodyEngine
{
public:
    static constexpr T VANGLE = T(PI / SPEED_OF_LIGHT);
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
    const T speedOfLightInv() const;

    double modelScale() const;
    void setModelScale(double);
    void setModelScaleRatio(double);
    void changeModelScale(double);
    double scaleInv() const;

    QVector<bhs::IntRange> timeProgressRanges() const;
    QVector<bhs::IntRange> interactionRanges() const;

    T velocityToAngle(T) const;
    void angleToVelocity(Vector3<T>&) const;
    void LorentzTransformation(QGenericMatrix<4, 4, T>&, T, T, T) const;
    void LorentzTransformation(QGenericMatrix<4, 4, T>&, const Vector3<T>&) const;

protected:
    void setNumberOfParticles();
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
    double m_scaleInv;
    T m_gravitationalConstant;
    QVector<bhs::IntRange> m_timeProgressRanges;
    QVector<bhs::IntRange> m_interactionRanges;
};
