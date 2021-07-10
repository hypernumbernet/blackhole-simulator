#pragma once

#include "quaternion.h"
#include "abstractnbodyengine.h"

#include <QObject>

using namespace bhs;

template <typename T>
class G3D4DEulerCalculation
{
public:
    G3D4DEulerCalculation(AbstractNBodyEngine<T>* const engine, const int threadNumber)
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

        const T vangleInv = AbstractNBodyEngine<T>::SPEED_OF_LIGHT / AbstractNBodyEngine<T>::PI;
        const T timePerFrame = m_engine->timePerFrame();

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            auto vq = Quaternion<T>(velocities, i * 4);
            vq.Normalize();
            auto vv3 = vq.LnV3();

            auto to_add = vv3 * vangleInv * timePerFrame;

            quint64 j = i * 3;
            coordinates[j] += to_add.x; ++j;
            coordinates[j] += to_add.y; ++j;
            coordinates[j] += to_add.z;
        }
    }

    inline void calculateInteraction() const
    {
        const T* const coordinates = m_engine->coordinates();
        T* const velocities = m_engine->velocities();
        const T* const masses = m_engine->masses();

        const T vangle = AbstractNBodyEngine<T>::PI / AbstractNBodyEngine<T>::SPEED_OF_LIGHT;
        const T vangleHalf = vangle * 0.5;
        const T timePerFrame = m_engine->timePerFrame();
        const T gravitationalConstant = m_engine->m_gravitationalConstant;
        const T timeG = timePerFrame * gravitationalConstant;
        const T numberOfParticles = m_engine->numberOfParticle();

        T d1, d2, d3, r, inv, theta;
        quint64 a, b;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            a = i * 3;

            T total_x = 0.0;
            T total_y = 0.0;
            T total_z = 0.0;

            for (quint64 j = 0; j < numberOfParticles; ++j)
            {
                if (i == j)
                    continue;

                b = j * 3;

                d1 = coordinates[b    ] - coordinates[a    ];
                d2 = coordinates[b + 1] - coordinates[a + 1];
                d3 = coordinates[b + 2] - coordinates[a + 2];

                r = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                if (r <= 0.0)
                    continue;

                inv = 1.0 / r;
                theta = inv * inv * inv * timeG * masses[j];

                d1 *= theta;
                d2 *= theta;
                d3 *= theta;

                total_x += d1;
                total_y += d2;
                total_z += d3;
            }
            a = i * 4;

            r = sqrt(total_x * total_x + total_y * total_y + total_z * total_z);
            if (r <= 0.0)
                continue;

            total_x /= r;
            total_y /= r;
            total_z /= r;
            auto acc = Quaternion<T>::MakeRotation(total_x, total_y, total_z, r * vangleHalf);

            auto va = Quaternion<T>(velocities, a);

            va.Rotate8(acc);

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
