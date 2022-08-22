#pragma once

#include "engine/abstractnbodyengine.h"

/*
Inelastic collision

velocities: ua, ub -> va, vb
masses: ma, mb
coefficient of restitution: e

va = (emb(ub - ua) + maua + mbub)/(ma + mb)
vb = (ema(ua - ub) + maua + mbub)/(ma + mb)

*/

class CalculationGravityCollision
{
public:
    CalculationGravityCollision(
            AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : m_engine(engine)
        , m_timeProgresStart(engine->timeProgressRanges().at(threadNumber).start)
        , m_timeProgresEnd(engine->timeProgressRanges().at(threadNumber).end)
        , m_interactionStart(engine->interactionRanges().at(threadNumber).start)
        , m_interactionEnd(engine->interactionRanges().at(threadNumber).end)
        , m_restitution(0.9)
        , m_boundary(0.04)
    {}

    void calculateTimeProgress() const
    {
        double* const coordinates = m_engine->coordinates();
        const double* const velocities = m_engine->velocities();

        const double timePerFrame = m_engine->timePerFrame();

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            quint64 j = i * 3;
            coordinates[j] += velocities[j] * timePerFrame; ++j;
            coordinates[j] += velocities[j] * timePerFrame; ++j;
            coordinates[j] += velocities[j] * timePerFrame;
        }
    }

    void calculateInteraction() const
    {
        const double* const coordinates = m_engine->coordinates();
        double* const velocities = m_engine->velocities();
        const double* const masses = m_engine->masses();
        double* const flags = m_engine->distances();

        const double timePerFrame = m_engine->timePerFrame();
        const double gravitationalConstant = m_engine->gravitationalConstant();
        const double timeG = timePerFrame * gravitationalConstant;
        const quint64 numberOfParticles = m_engine->numberOfParticle();
        const double boundaryToInvalidate = AbstractNBodyEngine<double>::BOUNDARY_TO_INVALIDATE;
        const double boundaryToCollision = m_boundary;

        double d1, d2, d3, r, inv, theta;
        double ua, ub, va, vb, ma, mb, mm, absa, absb, cosad, cosbd;
        quint64 a, b;
        Vector3 udv, uav, ubv, pav, pbv, qav, qbv;

        double* vels = new double[numberOfParticles * 3]();

        quint64 k = m_interactionStart * numberOfParticles - (m_interactionStart + 1) * m_interactionStart / 2;
        bool collision = false;

        for (quint64 i = m_interactionStart; i < m_interactionEnd; ++i)
        {
            a = i * 3;
            for (quint64 j = i + 1; j < numberOfParticles; ++j)
            {
                b = j * 3;

                d1 = coordinates[b    ] - coordinates[a    ];
                d2 = coordinates[b + 1] - coordinates[a + 1];
                d3 = coordinates[b + 2] - coordinates[a + 2];

                r = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                if (r <= boundaryToInvalidate)
                    continue;
                if (r <= boundaryToCollision)
                {
                    if (flags[k] == 1.0)
                        collision = false;
                    else {
                        collision = true;
                        flags[k++] = 1.0;
                    }
                } else {
                    collision = false;
                    flags[k++] = 0.0;
                }
                if (collision)
                {
                    udv.set(d1, d2, d3);
                    udv.normalize();
                    uav.set(velocities, a);
                    ubv.set(velocities, b);

                    absa = uav.abs();
                    absb = ubv.abs();
                    cosad = udv.dot(uav) / absa;
                    cosbd = udv.dot(ubv) / absb;
                    if (abs(cosad) >= 0.999999999) {
                        ua = absa;
                        qav.set(0.0, 0.0, 0.0);
                    } else {
                        ua = absa * cosad;
                        pav.set(uav.cross(udv));
                        qav.set(udv.cross(pav));
                        qav.normalize();
                        qav *= absa * sqrt(1.0 - cosad * cosad);
                    }
                    if (abs(cosbd) >= 0.999999999) {
                        ub = absb;
                        qbv.set(0.0, 0.0, 0.0);
                    } else {
                        ub = absb * cosbd;
                        pbv.set(ubv.cross(udv));
                        qbv.set(udv.cross(pbv));
                        qbv.normalize();
                        qbv *= absb * sqrt(1.0 - cosbd * cosbd);
                    }
                    ma = masses[i];
                    mb = masses[j];
                    mm = ma + mb;
                    va = (m_restitution * mb  * (ub - ua) + ma * ua + mb * ub) / mm;
                    vb = (m_restitution * ma  * (ua - ub) + ma * ua + mb * ub) / mm;

                    uav.set( udv * va + qav);
                    ubv.set(-udv * vb + qbv);

                    velocities[a    ] = uav.x();
                    velocities[a + 1] = uav.y();
                    velocities[a + 2] = uav.z();
                    velocities[b    ] = ubv.x();
                    velocities[b + 1] = ubv.y();
                    velocities[b + 2] = ubv.z();
                } else {
                    inv = 1.0 / r;
                    theta = inv * inv * inv * timeG;

                    d1 *= theta;
                    d2 *= theta;
                    d3 *= theta;

                    vels[a    ] += d1 * masses[j];
                    vels[a + 1] += d2 * masses[j];
                    vels[a + 2] += d3 * masses[j];
                    vels[b    ] -= d1 * masses[i];
                    vels[b + 1] -= d2 * masses[i];
                    vels[b + 2] -= d3 * masses[i];
                }
            }
        }
        bhs::interactionMutex.lock();
        quint64 end = numberOfParticles * 3;
        for (quint64 i = 0; i < end; ++i)
        {
            velocities[i] += vels[i];
        }
        bhs::interactionMutex.unlock();

        delete[] vels;
    }

private:
    AbstractNBodyEngine<double>* const m_engine;

    const quint64 m_timeProgresStart;
    const quint64 m_timeProgresEnd;
    const quint64 m_interactionStart;
    const quint64 m_interactionEnd;
    const double m_restitution;
    const double m_boundary;
};
