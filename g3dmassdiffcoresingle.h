#pragma once

#include "abstractenginecoresingle.h"

class G3DMassDiffCoreSingle : public AbstractEngineCoreSingle
{
    Q_OBJECT
public:
    explicit G3DMassDiffCoreSingle(AbstractNBodyEngine<float>* const engine, QObject* parent = nullptr)
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
        quint64 start = m_engine->interactionRanges().at(threadNumber).start;
        quint64 end = m_engine->interactionRanges().at(threadNumber).end;

        float d1, d2, d3, distance, inv, theta;
        quint64 k = 0, a, b;
        float time_g = m_timePerFrame * AbstractNBodyEngine<float>::GRAVITATIONAL_CONSTANT;

        float* vels = new float[m_numberOfParticles * 3];
        for (quint64 i = 0; i < m_numberOfParticles * 3; ++i) {
            vels[i] = 0.0f;
        }

        for (quint64 i = start; i < end; ++i)
        {
            for (quint64 j = i + 1; j < m_numberOfParticles; ++j)
            {
                // Perform differential calculation of universal gravitation.
                a = i * 3;
                b = j * 3;
                d1 = m_coordinates[a] - m_coordinates[b];
                d2 = m_coordinates[a + 1] - m_coordinates[b + 1];
                d3 = m_coordinates[a + 2] - m_coordinates[b + 2];
                distance = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                if (distance <= 0.0f) {
                    continue;
                }
                inv = 1.0f / distance;
                theta = inv * inv * time_g;
                //Q_ASSERT(theta == theta);

                d1 *= inv * theta;
                d2 *= inv * theta;
                d3 *= inv * theta;

                vels[a] -= d1 * m_masses[j];
                vels[a + 1] -= d2 * m_masses[j];
                vels[a + 2] -= d3 * m_masses[j];
                vels[b] += d1 * m_masses[i];
                vels[b + 1] += d2 * m_masses[i];
                vels[b + 2] += d3 * m_masses[i];

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
