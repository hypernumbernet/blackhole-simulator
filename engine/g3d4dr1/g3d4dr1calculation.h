#pragma once

#include "abstractnbodyengine.h"

#include <QObject>
#include <QDebug>

using namespace hnn;

template <typename T>
class G3D4DR1Calculation
{
public:
    G3D4DR1Calculation(AbstractNBodyEngine<T>* const engine, const int threadNumber)
        : m_engine(engine)
        , m_timeProgresStart(engine->timeProgressRanges().at(threadNumber).start)
        , m_timeProgresEnd(engine->timeProgressRanges().at(threadNumber).end)
        , m_interactionStart(engine->interactionRanges().at(threadNumber).start)
        , m_interactionEnd(engine->interactionRanges().at(threadNumber).end)
        , m_ct(T(SPEED_OF_LIGHT * engine->scaleInv()) * engine->timePerFrame())
    {
    }

    inline void calculateTimeProgress() const
    {
        T* const coordinates = m_engine->coordinates();
        const T* const velocities = m_engine->velocities();

        quint64 i4, i3;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            i4 = i * 4;
            i3 = i * 3;
            T tau = m_ct / velocities[i4];
            tau *= tau;
            if (!std::isfinite(tau))
            {
                //qDebug() << "tau: " << tau;
                continue;
            }
            coordinates[i3    ] += velocities[i4 + 1] * tau;
            coordinates[i3 + 1] += velocities[i4 + 2] * tau;
            coordinates[i3 + 2] += velocities[i4 + 3] * tau;
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
        quint64 numberOfParticles = m_engine->numberOfParticle();
        const T boundaryToInvalidate = AbstractNBodyEngine<T>::BOUNDARY_TO_INVALIDATE;

        T d1, d2, d3, r, inv, theta;
        quint64 i3, j3, i4;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            i3 = i * 3;
            i4 = i * 4;

            QGenericMatrix<1, 4, T> speed;
            speed(0, 0) = velocities[i4    ];
            speed(1, 0) = velocities[i4 + 1];
            speed(2, 0) = velocities[i4 + 2];
            speed(3, 0) = velocities[i4 + 3];

            QGenericMatrix<4, 4, T> acc;

            for (quint64 j = 0; j < numberOfParticles; ++j)
            {
                if (i == j)
                    continue;

                j3 = j * 3;

                d1 = coordinates[j3    ] - coordinates[i3    ];
                d2 = coordinates[j3 + 1] - coordinates[i3 + 1];
                d3 = coordinates[j3 + 2] - coordinates[i3 + 2];

                r = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                if (r <= boundaryToInvalidate)
                    continue;

                inv = T(1.0) / r;
                theta = inv * inv * inv * timeG * masses[j];
                d1 *= theta;
                d2 *= theta;
                d3 *= theta;

                Vector3<T> g(-d1, -d2, -d3);
                m_engine->LorentzTransformation(acc, g);
                speed = acc * speed;
            }
            velocities[i4    ] = speed(0, 0);
            velocities[i4 + 1] = speed(1, 0);
            velocities[i4 + 2] = speed(2, 0);
            velocities[i4 + 3] = speed(3, 0);
        }
    }

private:
    AbstractNBodyEngine<T>* const m_engine;

    const quint64 m_timeProgresStart;
    const quint64 m_timeProgresEnd;
    const quint64 m_interactionStart;
    const quint64 m_interactionEnd;

    const T m_ct;
};
