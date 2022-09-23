#pragma once

#include "engine/AbstractNBodyEngine.h"
#include "InitializerUniverse2.h"

using namespace hnn;

/**
 * @brief Quaternion Logarithm Gravity
 * Galactic
 */
class CalculationUniverse2
{
public:
    CalculationUniverse2(AbstractNBodyEngine<double>* const engine, const int threadNumber)
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
        double* const locations = m_engine->locations();
        const double angleToCoord = InitializerUniverse2<double>::ZOOM / PI;

        quint64 i3, i4;
        Quaternion vq, lq;
        Vector3 vv, lv;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            i3 = i * 3;
            i4 = i * 4;
            vv.set(velocities, i3);
            vv *= timePerFrame;
            vq.set(Quaternion::exp(vv));
            lq.set(locations, i4);
            lq = vq * lq;
            bhs::embedQuaternionToArray<double>(lq, locations, i4);
            lv.set(lq.lnV3() * angleToCoord);
            bhs::embedVector3ToArray<double>(lv, coordinates, i3);
        }
    }

    void calculateInteraction() const
    {
        const double* const coordinates = m_engine->coordinates();
        double* const velocities = m_engine->velocities();
        const double* const masses = m_engine->masses();
        double* const distances = m_engine->distances();
        const quint64 numberOfParticles = m_engine->numberOfParticle();
        const double timePerFrame = m_engine->timePerFrame();
        const double gravitationalConstant = m_engine->gravitationalConstant();
        const double timeG = timePerFrame * gravitationalConstant;
        const quint64 number3 = numberOfParticles * 3;

        quint64 k = m_interactionStart * numberOfParticles - (m_interactionStart + 1) * m_interactionStart / 2;

        double d1, d2, d3, r, theta, ma, mb;
        quint64 i, i3, j3;

        double* acceleration = new double[number3]();

        for (i = m_interactionStart; i < m_interactionEnd; ++i)
        {
            i3 = i * 3;
            for (quint64 j = i + 1; j < numberOfParticles; ++j)
            {
                j3 = j * 3;

                d1 = coordinates[j3    ] - coordinates[i3    ];
                d2 = coordinates[j3 + 1] - coordinates[i3 + 1];
                d3 = coordinates[j3 + 2] - coordinates[i3 + 2];

                r = sqrt(d1 * d1 + d2 * d2 + d3 * d3);

                theta = distances[k];
                distances[k] = 1.0 / r;
                theta -= distances[k];

                if (std::isfinite(theta))
                {
                    theta = fabs(theta);
                    theta *= timeG;
                    d1 *= theta;
                    d2 *= theta;
                    d3 *= theta;
                    ma = masses[i];
                    mb = masses[j];
                    acceleration[i3    ] += d1 * mb;
                    acceleration[i3 + 1] += d2 * mb;
                    acceleration[i3 + 2] += d3 * mb;
                    acceleration[j3    ] -= d1 * ma;
                    acceleration[j3 + 1] -= d2 * ma;
                    acceleration[j3 + 2] -= d3 * ma;
                } else {
                    distances[k] = std::numeric_limits<double>::max();
                }
                ++k;
            }
        }
        bhs::interactionMutex.lock();
        for (i = 0; i < number3; ++i)
        {
            r = velocities[i];
            velocities[i] += acceleration[i];
            if ( ! std::isfinite(velocities[i]))
            {
                //qDebug() << "VEL" << i << vels[i];
                velocities[i] = r;
            }
        }
        bhs::interactionMutex.unlock();

        delete[] acceleration;
    }

private:
    AbstractNBodyEngine<double>* const m_engine;

    const quint64 m_timeProgresStart;
    const quint64 m_timeProgresEnd;
    const quint64 m_interactionStart;
    const quint64 m_interactionEnd;
};
