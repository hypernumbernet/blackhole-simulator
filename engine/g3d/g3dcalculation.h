#pragma once

#include <QObject>

#include "abstractnbodyengine.h"

template <typename T>
class G3DCalculation
{
public:
    G3DCalculation(AbstractNBodyEngine<T>* const engine, const int threadNumber)
        : m_engine(engine)
        , m_timeProgresStart(engine->timeProgressRanges().at(threadNumber).start)
        , m_timeProgresEnd(engine->timeProgressRanges().at(threadNumber).end)
        , m_interactionStart(engine->interactionRanges().at(threadNumber).start)
        , m_interactionEnd(engine->interactionRanges().at(threadNumber).end)
    {}

    inline void calculateTimeProgress() const
    {
        T* const coordinates = m_engine->coordinates();
        const T* const velocities = m_engine->velocities();

        const T timePerFrame = m_engine->timePerFrame();

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            quint64 j = i * 3;
            coordinates[j] += velocities[j] * timePerFrame; ++j;
            coordinates[j] += velocities[j] * timePerFrame; ++j;
            coordinates[j] += velocities[j] * timePerFrame;
        }
    }

    inline void calculateInteraction() const
    {
        const T* const coordinates = m_engine->coordinates();
        T* const velocities = m_engine->velocities();
        const T* const masses = m_engine->masses();

        const T timePerFrame = m_engine->timePerFrame();
        const T gravitationalConstant = m_engine->gravitationalConstant();
        const T timeG = timePerFrame * gravitationalConstant;
        const quint64 numberOfParticles = m_engine->numberOfParticle();
        const T boundaryToInvalidate = AbstractNBodyEngine<T>::BOUNDARY_TO_INVALIDATE;

        T d1, d2, d3, r, inv, theta;
        quint64 a, b;

        T* vels = new T[numberOfParticles * 3]();

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
    AbstractNBodyEngine<T>* const m_engine;

    const quint64 m_timeProgresStart;
    const quint64 m_timeProgresEnd;
    const quint64 m_interactionStart;
    const quint64 m_interactionEnd;
};
