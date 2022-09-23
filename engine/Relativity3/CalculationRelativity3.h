#pragma once

#include "engine/AbstractNBodyEngine.h"
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

        quint64 i4, i3;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            i4 = i * 4;
            i3 = i * 3;
            double tau = m_ct / velocities[i4];
            if ( ! std::isfinite(tau))
            {
                //qDebug() << "tau: " << tau << i;
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
        const quint64 numberOfParticles = m_engine->numberOfParticle();
        const double boundaryToInvalidate = AbstractNBodyEngine<double>::BOUNDARY_TO_INVALIDATE;

        quint64 i, j, a, b, i4;
        Vector3 acceleration, acc;
        Quaternion qd;
        double d1, d2, d3, r, inv, theta;
        double accAbs;
        double accBetaAngle;
        double sinBeta;

        static const int w = 1;
        static const int x = 2;
        static const int y = 4;
        static const int z = 6;
        Octonion origin(0.);
        origin[w] = 1.;
        Octonion ov;
        Octonion od;
        Octonion pole;
        Octonion rotation;

        for (i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            a = i * 3;
            i4 = i * 4;

            acceleration.set(0., 0., 0.);

            for (j = 0; j < numberOfParticles; ++j)
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
                theta = inv * inv * inv * timeG * masses[j];
                acc.set(d1 * theta, d2 * theta, d3 * theta);
                acceleration += acc;
            }

            accAbs = acceleration.abs();
            if (accAbs == 0.)
                continue;
            acceleration /= accAbs;
            accBetaAngle = 0.5 * PI * accAbs * m_speedOfLightInv;
            sinBeta = sin(accBetaAngle);
            qd.setRe(cos(accBetaAngle));
            qd.setI1(sinBeta * acceleration.x());
            qd.setI2(sinBeta * acceleration.y());
            qd.setI3(sinBeta * acceleration.z());


            ov.set(0.);
            ov[w] = velocities[i4    ];
            ov[x] = velocities[i4 + 1];
            ov[y] = velocities[i4 + 2];
            ov[z] = velocities[i4 + 3];

            qd.set(Quaternion::rotation(acceleration, 1.));
            od.set(0.);
            od[w] = qd.re();
            od[x] = qd.i1();
            od[y] = qd.i2();
            od[z] = qd.i3();
            pole.set(od.cross(origin));
            rotation.set(Octonion::rotation(pole, accBetaAngle));
            ov.set(rotation.conjugated() * ov * rotation);

            velocities[i4    ] = ov[w];
            velocities[i4 + 1] = ov[x];
            velocities[i4 + 2] = ov[y];
            velocities[i4 + 3] = ov[z];
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
