#pragma once

#include "engine/abstractnbodyengine.h"

class CalculationGravityElasticity
{
public:
    CalculationGravityElasticity(
            AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : m_engine(engine)
        , m_timeProgresStart(engine->timeProgressRanges().at(threadNumber).start)
        , m_timeProgresEnd(engine->timeProgressRanges().at(threadNumber).end)
        , m_interactionStart(engine->interactionRanges().at(threadNumber).start)
        , m_interactionEnd(engine->interactionRanges().at(threadNumber).end)
    {}

    void calculateTimeProgress() const
    {
        double* const coordinates = m_engine->coordinates();
        const double* const velocities = m_engine->velocities();

        const double timePerFrame = m_engine->timePerFrame();

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            quint64 j = i * 3;
            coordinates[j] += velocities[j] * timePerFrame; ++j;
            coordinates[j] += velocities[j] * timePerFrame; ++j;
            coordinates[j] += velocities[j] * timePerFrame;
        }
    }

    void calculateInteraction() const
    {
        static const double CONTINUE_THRESHOLD = 1e-10;
        static const double BOUNDARY_THRESHOLD = 0.04;
        static const double ELASTIC_MODULUS = 1e-4;

        double* const coordinates = m_engine->coordinates();
        double* const velocities = m_engine->velocities();
        const double* const masses = m_engine->masses();

        const double timePerFrame = m_engine->timePerFrame();
        const double gravitationalConstant = m_engine->gravitationalConstant();
        const double scaleInv = m_engine->scaleInv();
        const double scaleBound = ELASTIC_MODULUS * scaleInv * scaleInv * scaleInv / BOUNDARY_THRESHOLD;
        const double timeG = timePerFrame * gravitationalConstant;
        const quint64 numberOfParticles = m_engine->numberOfParticle();

        double r, inv, ma, mb, effect;
        quint64 a, b;
        Vector3 coa, cob, cod, dir, vav, vbv;

        double* vels = new double[numberOfParticles * 3]();

        for (quint64 i = m_interactionStart; i < m_interactionEnd; ++i)
        {
            a = i * 3;
            for (quint64 j = i + 1; j < numberOfParticles; ++j)
            {
                b = j * 3;
                coa.set(coordinates, a);
                cob.set(coordinates, b);
                cod.set(cob - coa);
                r = cod.abs();
                if (r < CONTINUE_THRESHOLD)
                {
                    //qDebug() << "continue r" << a << b;
                    continue;
                }
                ma = masses[i];
                mb = masses[j];
                if (r < BOUNDARY_THRESHOLD)
                {
                    dir.set(cod.normalized());
                    effect = (BOUNDARY_THRESHOLD - r) * scaleBound;
                    vav.set(dir * (-effect / ma));
                    vbv.set(dir * ( effect / mb));
                    if (vav.isfinite() && vbv.isfinite()) {
                        vels[a    ] += vav.x();
                        vels[a + 1] += vav.y();
                        vels[a + 2] += vav.z();
                        vels[b    ] += vbv.x();
                        vels[b + 1] += vbv.y();
                        vels[b + 2] += vbv.z();
                    } else {
                        //qDebug() << "CLL" << a << b << "vav" << vav.toString().c_str() << "vbv" << vbv.toString().c_str();
                    }
                    continue;
                }
                inv = 1.0 / r;
                effect = inv * inv * inv * timeG;
                vav.set(cod * ( effect * mb));
                vbv.set(cod * (-effect * ma));
                //if ( ! vav.isfinite() || ! vbv.isfinite()) {
                //    qDebug() << "GRV" << a << b << "vav" << vav.toString().c_str() << "vbv" << vbv.toString().c_str();
                //    continue;
                //}
                vels[a    ] += vav.x();
                vels[a + 1] += vav.y();
                vels[a + 2] += vav.z();
                vels[b    ] += vbv.x();
                vels[b + 1] += vbv.y();
                vels[b + 2] += vbv.z();
            }
        }
        bhs::interactionMutex.lock();
        quint64 end = numberOfParticles * 3;
        for (quint64 i = 0; i < end; ++i)
        {
            velocities[i] += vels[i];
        }
        bhs::interactionMutex.unlock();

        delete[] vels;
    }

private:
    AbstractNBodyEngine<double>* const m_engine;

    const quint64 m_timeProgresStart;
    const quint64 m_timeProgresEnd;
    const quint64 m_interactionStart;
    const quint64 m_interactionEnd;
};
