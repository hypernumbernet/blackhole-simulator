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
        const double* const angles = m_engine->velocities();
        quint64 i3;
        Vector3 angle;
        Spacetime st;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            i3 = i * 3;
            angle.set(angles[i3], angles[i3 + 1], angles[i3 + 2]);

            st.set(m_ct);
            st.lorentzTransformation(angle);
            double tau = m_ct / st.w();
            //tau *= tau;
            if ( ! std::isfinite(tau))
            {
                //qDebug() << "tau: " << tau << i;
                continue;
            }
            coordinates[i3    ] += st.x() * tau;
            coordinates[i3 + 1] += st.y() * tau;
            coordinates[i3 + 2] += st.z() * tau;

            //angle.set(Spacetime::velocities(angle, m_ct));
            //if ( ! isfinite(angle.x()))
            //{
            //    qDebug() << "angles" << i << angle;
            //}
            //coordinates[i3    ] += angle.x();
            //coordinates[i3 + 1] += angle.y();
            //coordinates[i3 + 2] += angle.z();
        }
    }

    void calculateInteraction() const
    {
        const double* const coordinates = m_engine->coordinates();
        double* const angles = m_engine->velocities();
        const double* const masses = m_engine->masses();

        const double timePerFrame = m_engine->timePerFrame();
        const double gravitationalConstant = m_engine->gravitationalConstant();
        const double timeG = timePerFrame * gravitationalConstant;
        const quint64 numberOfParticles = m_engine->numberOfParticle();
        const quint64 number3 = numberOfParticles * 3;
        const double boundaryToInvalidate = AbstractNBodyEngine<double>::BOUNDARY_TO_INVALIDATE;
        const double speedOfLight = SPEED_OF_LIGHT * m_engine->scaleInv();

        double d1, d2, d3, r, inv, theta, ma, mb;
        quint64 i, j, a, b;
        Vector3 momentumDelta, angleA, angleB;

        double* deltaAngles = new double[number3]();

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
                if (r <= boundaryToInvalidate)
                    continue;

                inv = 1.0 / r;
                ma = masses[i];
                mb = masses[j];
                theta = inv * inv * inv * timeG * ma * mb;

                d1 *= theta;
                d2 *= theta;
                d3 *= theta;
                momentumDelta.set(d1, d2, d3);
                angleA.set(Spacetime::versorAngle(  momentumDelta, ma, speedOfLight));
                angleB.set(Spacetime::versorAngle(- momentumDelta, mb, speedOfLight));
                deltaAngles[a    ] += angleA.x();
                deltaAngles[a + 1] += angleA.y();
                deltaAngles[a + 2] += angleA.z();
                deltaAngles[b    ] += angleB.x();
                deltaAngles[b + 1] += angleB.y();
                deltaAngles[b + 2] += angleB.z();
            }
        }
        bhs::interactionMutex.lock();
        for (i = 0; i < number3; ++i)
        {
            r = angles[i];
            angles[i] += deltaAngles[i];
            if ( ! std::isfinite(angles[i]))
            {
                //qDebug() << "angles" << i << angles[i];
                angles[i] = r;
            }
        }
        bhs::interactionMutex.unlock();

        delete[] deltaAngles;
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
