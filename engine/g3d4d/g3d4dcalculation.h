#pragma once

#include <QObject>
#include <QDebug>

#include "abstractnbodyengine.h"

using namespace hnn;

template <typename T>
class G3D4DCalculation
{
public:
    G3D4DCalculation(AbstractNBodyEngine<T>* const engine, const int threadNumber)
        : m_engine(engine)
        , m_timeProgresStart(engine->timeProgressRanges().at(threadNumber).start)
        , m_timeProgresEnd(engine->timeProgressRanges().at(threadNumber).end)
        , m_interactionStart(engine->interactionRanges().at(threadNumber).start)
        , m_interactionEnd(engine->interactionRanges().at(threadNumber).end)
    {
    }

    inline void calculateTimeProgress() const
    {
        T* const coordinates = m_engine->coordinates();
        const T* const velocities = m_engine->velocities();

        const T timePerFrame = m_engine->timePerFrame();

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            auto vq = Quaternion<T>(velocities, i * 4);
            auto vv3 = vq.LnV3();
            m_engine->angleToVelocity(vv3);
            auto to_add = vv3 * timePerFrame;

            quint64 j = i * 3;
            coordinates[j] += to_add.x(); ++j;
            coordinates[j] += to_add.y(); ++j;
            coordinates[j] += to_add.z();
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
        quint64 a, b;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            a = i * 3;

            auto qi = Quaternion<T>::Identity;

            for (quint64 j = 0; j < numberOfParticles; ++j)
            {
                if (i == j)
                    continue;

                b = j * 3;

                d1 = coordinates[b    ] - coordinates[a    ];
                d2 = coordinates[b + 1] - coordinates[a + 1];
                d3 = coordinates[b + 2] - coordinates[a + 2];

                r = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                if (r <= boundaryToInvalidate)
                    continue;

                inv = 1.0 / r;
                d1 *= inv;
                d2 *= inv;
                d3 *= inv;
                theta = inv * inv * timeG * masses[j];

                theta = m_engine->velocityToAngle(theta);
                auto acc = Quaternion<T>::MakeRotation(d1, d2, d3, theta * T(0.5));
                qi.Rotate8(acc);
            }
            a = i * 4;

            auto va = Quaternion<T>(velocities, a);
            va *= qi;

            velocities[a    ] = va.i0;
            velocities[a + 1] = va.i1;
            velocities[a + 2] = va.i2;
            velocities[a + 3] = va.i3;
        }
    }

private:
    AbstractNBodyEngine<T>* const m_engine;

    const quint64 m_timeProgresStart;
    const quint64 m_timeProgresEnd;
    const quint64 m_interactionStart;
    const quint64 m_interactionEnd;
};
