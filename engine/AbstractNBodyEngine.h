#pragma once

#include "bhs.h"
#include "hnn/vector3.h"
#include <QThread>
#include <QVector4D>
#include <QGenericMatrix>

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
    T* distances() const;
    T* locations() const;
    T* accelerations() const;

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

    double velocityToAngle(double) const;
    void angleToVelocity(Vector3&) const;

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

    T* m_distances;

    T* m_locations;

    T* m_accelerations;

    quint64 m_numberOfParticles;
    double m_modelScale;
    double m_scaleInv;
    T m_gravitationalConstant;
    QVector<bhs::IntRange> m_timeProgressRanges;
    QVector<bhs::IntRange> m_interactionRanges;
};
