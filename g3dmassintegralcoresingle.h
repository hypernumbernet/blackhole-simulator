#pragma once

#include "abstractenginecoresingle.h"
#include "g3dmassintegralnbe.h"

class G3DMassIntegralCoreSingle : public AbstractEngineCoreSingle
{
    Q_OBJECT

public:
    explicit G3DMassIntegralCoreSingle(AbstractNBodyEngine<float>* const engine, QObject* parent = nullptr)
        : AbstractEngineCoreSingle(engine, parent)
    {
    }

public slots:
    void calculateTimeProgress(int threadNumber) const
    {
        quint64 start = m_engine->timeProgressRanges().at(threadNumber).start;
        quint64 end = m_engine->timeProgressRanges().at(threadNumber).end;

        for (quint64 i = start; i < end; ++i)
        {
            quint64 j = i * 3;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame;
        }
        resultReady();
    }

    void calculateInteraction(int threadNumber) const
    {
        // Perform integral calculation of universal gravitation.
        // The memory cost is high because the distance data calculated last time is saved and used.
        // However, if the distance variation is large, it may be more accurate to calculate by this method.
        // The celestial scale vibrates violently at float resolution?

        quint64 start = m_engine->interactionRanges().at(threadNumber).start;
        quint64 end = m_engine->interactionRanges().at(threadNumber).end;

        float inv, force;
        G3DMassIntegralNBE<float>::Distance d;
        quint64 k = 0, a, b;

        float* vels = new float[m_numberOfParticles * 3];
        for (quint64 i = 0; i < m_numberOfParticles * 3; ++i) {
            vels[i] = 0.0f;
        }

        for (quint64 i = start; i < end; ++i)
        {
            for (quint64 j = i + 1; j < m_numberOfParticles; ++j)
            {
                if (!G3DMassIntegralNBE<float>::calculateDistance(d, m_coordinates, i, j))
                    continue;

                inv = m_inversedDistances[k];
                m_inversedDistances[k] = d.invR;
                inv -= d.invR;

                if (inv == 0.0f)
                {
                    continue;
                }

                // Time is not taken into account.
                force = inv * AbstractNBodyEngine<float>::GRAVITATIONAL_CONSTANT;

                d.unitX *= force;
                d.unitY *= force;
                d.unitZ *= force;

                a = i * 3;
                b = j * 3;
                vels[a    ] -= d.unitX * m_masses[j];
                vels[a + 1] -= d.unitY * m_masses[j];
                vels[a + 2] -= d.unitZ * m_masses[j];
                vels[b    ] += d.unitX * m_masses[i];
                vels[b + 1] += d.unitY * m_masses[i];
                vels[b + 2] += d.unitZ * m_masses[i];

                ++k;
            }
        }

        bhs::interactionMutex.lock();
        for (quint64 i = 0; i < m_numberOfParticles * 3; ++i) {
            m_velocities[i] += vels[i];
        }
        bhs::interactionMutex.unlock();

        delete[] vels;

        resultReady();
    }
};
