#pragma once

#include "abstractenginecoredouble.h"
#include "g3dmassintegralnbe.h"

class G3DMassIntegralCoreDouble : public AbstractEngineCoreDouble
{
    Q_OBJECT

public:
    explicit G3DMassIntegralCoreDouble(AbstractNBodyEngine<double>* const engine, QObject* parent = nullptr)
        : AbstractEngineCoreDouble(engine, parent)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine)
    {
        return new G3DMassIntegralCoreDouble(engine);
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

        double inv, force;
        G3DMassIntegralNBE<double>::Distance d;
        quint64 k = 0, a, b;

        double* vels = new double[m_numberOfParticles * 3];
        for (quint64 i = 0; i < m_numberOfParticles * 3; ++i) {
            vels[i] = 0.0;
        }

        for (quint64 i = start; i < end; ++i)
        {
            for (quint64 j = i + 1; j < m_numberOfParticles; ++j)
            {
                if (!G3DMassIntegralNBE<double>::calculateDistance(d, m_coordinates, i, j))
                    continue;

                inv = m_inversedDistances[k];
                m_inversedDistances[k] = d.invR;
                inv -= d.invR;

                if (inv == 0.0)
                    continue;

                // Time is not taken into account.
                force = inv * AbstractNBodyEngine<double>::GRAVITATIONAL_CONSTANT;

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
