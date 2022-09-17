#pragma once

#include "engine/abstractnbodyengine.h"
#include "hnn/octonion.h"

using namespace hnn;

class CalculationRelativity3
{
public:
    CalculationRelativity3(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : m_engine(engine)
        , m_timeProgresStart(engine->timeProgressRanges().at(threadNumber).start)
        , m_timeProgresEnd(engine->timeProgressRanges().at(threadNumber).end)
        , m_interactionStart(engine->interactionRanges().at(threadNumber).start)
        , m_interactionEnd(engine->interactionRanges().at(threadNumber).end)
        , m_ct(SPEED_OF_LIGHT * engine->scaleInv() * engine->timePerFrame())
        , m_speedOfLightInv(1.0 / (SPEED_OF_LIGHT * engine->scaleInv()))
    {
    }

    void calculateTimeProgress() const
    {
        double* const coordinates = m_engine->coordinates();
        double* const velocities = m_engine->velocities();
        const double timePerFrame = m_engine->timePerFrame();

        Quaternion vq;
        Vector3 v3;
        quint64 i3;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            vq.set(velocities, i * 4);
            v3.set(vq.lnV3());
            m_engine->angleToVelocity(v3);
            v3 *= timePerFrame;
            i3 = i * 3;
            coordinates[i3] += v3.x(); ++i3;
            coordinates[i3] += v3.y(); ++i3;
            coordinates[i3] += v3.z();
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
        const double boundaryToInvalidate = AbstractNBodyEngine<double>::BOUNDARY_TO_INVALIDATE;

        quint64 i, j, a, b, i4;
        Quaternion qd;
        double d1, d2, d3, r, inv, theta;

        static const int w = 1;
        static const int x = 2;
        static const int y = 4;
        static const int z = 6;
        Octonion origin(0.);
        Octonion ov;
        Octonion od;
        Octonion pole;
        Octonion rotation;
        origin[w] = 1.;

        for (i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            a = i * 3;
            i4 = i * 4;
            ov.set(0.);
            ov[w] = velocities[i4];
            ov[x] = velocities[i4 + 1];
            ov[y] = velocities[i4 + 2];
            ov[z] = velocities[i4 + 3];
            for (j = i + 1; j < numberOfParticles; ++j)
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

                qd.set(Quaternion::rotation(d1, d2, d3, 1.));
                od.set(0.);
                od[w] = qd.re();
                od[x] = qd.i1();
                od[y] = qd.i2();
                od[z] = qd.i3();
                pole.set(od.cross(origin));
                rotation = pole * sin(theta * 0.5);
                rotation.setRe(cos(theta * 0.5));



            }
        }
    }

private:
    AbstractNBodyEngine<double>* const m_engine;

    const quint64 m_timeProgresStart;
    const quint64 m_timeProgresEnd;
    const quint64 m_interactionStart;
    const quint64 m_interactionEnd;

    const double m_ct;
    const double m_speedOfLightInv;
};
