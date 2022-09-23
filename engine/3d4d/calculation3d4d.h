#pragma once

#include "engine/AbstractNBodyEngine.h"

using namespace hnn;

class Calculation3D4D
{
public:
    Calculation3D4D(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : m_engine(engine)
        , m_timeProgresStart(engine->timeProgressRanges().at(threadNumber).start)
        , m_timeProgresEnd(engine->timeProgressRanges().at(threadNumber).end)
        , m_interactionStart(engine->interactionRanges().at(threadNumber).start)
        , m_interactionEnd(engine->interactionRanges().at(threadNumber).end)
    {
    }

    void calculateTimeProgress() const
    {
        double* const coordinates = m_engine->coordinates();
        const double* const velocities = m_engine->velocities();

        const double timePerFrame = m_engine->timePerFrame();

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            auto vq = Quaternion(velocities, i * 4);
            auto vv3 = vq.lnV3();
            m_engine->angleToVelocity(vv3);
            auto to_add = vv3 * timePerFrame;

            quint64 j = i * 3;
            coordinates[j] += to_add.x(); ++j;
            coordinates[j] += to_add.y(); ++j;
            coordinates[j] += to_add.z();
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
        quint64 numberOfParticles = m_engine->numberOfParticle();
        const double boundaryToInvalidate = AbstractNBodyEngine<double>::BOUNDARY_TO_INVALIDATE;

        double d1, d2, d3, r, inv, theta;
        quint64 a, b;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            a = i * 3;

            auto qi = Quaternion::identity();

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
                auto acc = Quaternion::rotation(d1, d2, d3, theta);
                qi.rotate8(acc);
            }
            a = i * 4;

            auto va = Quaternion(velocities, a);
            va *= qi;

            velocities[a    ] = va.re();
            velocities[a + 1] = va.i1();
            velocities[a + 2] = va.i2();
            velocities[a + 3] = va.i3();
        }
    }

private:
    AbstractNBodyEngine<double>* const m_engine;

    const quint64 m_timeProgresStart;
    const quint64 m_timeProgresEnd;
    const quint64 m_interactionStart;
    const quint64 m_interactionEnd;
};
