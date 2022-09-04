#pragma once

#include "engine/abstractnbodyengine.h"

class Calculation3D
{
public:
    Calculation3D(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : m_engine(engine)
        , m_timeProgresStart(engine->timeProgressRanges().at(threadNumber).start)
        , m_timeProgresEnd(engine->timeProgressRanges().at(threadNumber).end)
        , m_interactionStart(engine->interactionRanges().at(threadNumber).start)
        , m_interactionEnd(engine->interactionRanges().at(threadNumber).end)
    {}

    void calculateTimeProgress() const
    {
        double* const coordinates = m_engine->coordinates();
        const double* const velocities = m_engine->velocities();
        const double timePerFrame = m_engine->timePerFrame();
        quint64 j;
        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            j = i * 3;
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

        const double timePerFrame = m_engine->timePerFrame();
        const double gravitationalConstant = m_engine->gravitationalConstant();
        const double timeG = timePerFrame * gravitationalConstant;
        const quint64 numberOfParticles = m_engine->numberOfParticle();
        const quint64 number3 = numberOfParticles * 3;
        const double boundaryToInvalidate = AbstractNBodyEngine<double>::BOUNDARY_TO_INVALIDATE;

        double d1, d2, d3, r, inv, theta, ma, mb;
        quint64 a, b, i, j;

        double* vels = new double[number3]();

        for (i = m_interactionStart; i < m_interactionEnd; ++i)
        {
            a = i * 3;
            for (j = i + 1; j < numberOfParticles; ++j)
            {
                b = j * 3;

                d1 = coordinates[b    ] - coordinates[a    ];
                d2 = coordinates[b + 1] - coordinates[a + 1];
                d3 = coordinates[b + 2] - coordinates[a + 2];

                r = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                if (r < boundaryToInvalidate)
                    continue;

                inv = 1.0 / r;
                theta = inv * inv * inv * timeG;

                d1 *= theta;
                d2 *= theta;
                d3 *= theta;
                ma = masses[i];
                mb = masses[j];
                vels[a    ] += d1 * mb;
                vels[a + 1] += d2 * mb;
                vels[a + 2] += d3 * mb;
                vels[b    ] -= d1 * ma;
                vels[b + 1] -= d2 * ma;
                vels[b + 2] -= d3 * ma;
            }
        }
        bhs::interactionMutex.lock();
        for (i = 0; i < number3; ++i)
        {
            r = velocities[i];
            velocities[i] += vels[i];
            if ( ! isfinite(velocities[i]))
            {
                //qDebug() << "VEL" << i << vels[i];
                velocities[i] = r;
            }
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
};
