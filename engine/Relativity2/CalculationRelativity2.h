#pragma once

#include "engine/abstractnbodyengine.h"

using namespace hnn;

class CalculationRelativity2
{
public:
    CalculationRelativity2(AbstractNBodyEngine<double>* const engine, const int threadNumber)
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
        const double* const velocities = m_engine->velocities();

        quint64 i4, i3;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            i4 = i * 4;
            i3 = i * 3;
            double tau = m_ct / velocities[i4];
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
        quint64 i3, j3, i4;
        Spacetime speed;
        Vector3 g;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            i3 = i * 3;
            i4 = i * 4;

            speed.setW(velocities[i4    ]);
            speed.setX(velocities[i4 + 1]);
            speed.setY(velocities[i4 + 2]);
            speed.setZ(velocities[i4 + 3]);

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

                inv = 1.0 / r;
                theta = inv * inv * inv * timeG * masses[j];
                d1 *= theta;
                d2 *= theta;
                d3 *= theta;

                g.set(-d1, -d2, -d3);
                speed.lorentzTransformation(g, m_speedOfLightInv);
            }
            velocities[i4    ] = speed.w();
            velocities[i4 + 1] = speed.x();
            velocities[i4 + 2] = speed.y();
            velocities[i4 + 3] = speed.z();
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
