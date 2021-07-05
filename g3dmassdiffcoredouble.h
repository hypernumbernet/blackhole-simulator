#pragma once

#include "abstractenginecoredouble.h"

class G3DMassDiffCoreDouble : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    explicit G3DMassDiffCoreDouble(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new G3DMassDiffCoreDouble(engine, threadNumber);
    }

public slots:
    void calculateTimeProgress() const
    {
        for (quint64 i = m_start; i < m_end; ++i)
        {
            quint64 j = i * 3;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame;
        }
        resultReady();
    }

    void calculateInteraction() const
    {
        double d1, d2, d3, distance, inv, theta;
        quint64 k = 0, a, b;
        double time_g = m_timePerFrame * m_engine->m_gravitationalConstant;

        double* vels = new double[m_numberOfParticles * 3]();
        //Q_ASSERT(vels[0] == 0.0);

        for (quint64 i = m_start; i < m_end; ++i)
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
                if (distance <= 0.0)
                    continue;

                inv = 1.0 / distance;
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
        for (quint64 i = 0; i < m_numberOfParticles * 3; ++i)
        {
            m_velocities[i] += vels[i];
        }
        bhs::interactionMutex.unlock();

        delete[] vels;

        resultReady();
    }
};
