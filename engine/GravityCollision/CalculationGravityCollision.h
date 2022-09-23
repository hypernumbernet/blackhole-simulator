#pragma once

#include "engine/AbstractNBodyEngine.h"

/*
Inelastic collision formula

Velocities: ua, ub -> va, vb
Masses: ma, mb
Coefficient of Restitution: e

va = (e mb(ub - ua) + ma ua + mb ub) / (ma + mb)
vb = (e ma(ua - ub) + ma ua + mb ub) / (ma + mb)

When ua -> 0
ud = ub - ua
va = (e + 1)mb ud / (ma + mb)

Thus
Impulsive force: ma va

ma va = ma (e + 1)mb ud / (ma + mb)
      = (e + 1) ud ma mb / (ma + mb)

Accelaration for a:   ma va / ma = va =   (e + 1) ud mb / (ma + mb)
Accelaration for b: - ma va / mb      = - (e + 1) ud ma / (ma + mb)

Difinition
effect = (e + 1) ud / (ma + mb)

Accelaration for a:   effect * mb
Accelaration for b: - effect * ma

'effect' means: momentum / (ma mb)
*/

class CalculationGravityCollision
{
public:
    CalculationGravityCollision(
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
        static const double BOUNCY_THRESHOLD = 0.04;
        static const double COS_MIN_THRESHOLD = - 1.0;
        static const double COS_MAX_THRESHOLD = 1.0;
        static const double ELASTIC_MODULUS = 0.8;
        static const double REPULSIVE_RATE = 30.0;

        double* const coordinates = m_engine->coordinates();
        double* const velocities = m_engine->velocities();
        const double* const masses = m_engine->masses();
        double* const flags = m_engine->distances();

        const double timePerFrame = m_engine->timePerFrame();
        const double gravitationalConstant = m_engine->gravitationalConstant();
        const quint64 numberOfParticles = m_engine->numberOfParticle();
        //const double scaleInv = m_engine->scaleInv();

        const double timeG = timePerFrame * gravitationalConstant;
        const double repulsive = - timeG * REPULSIVE_RATE;
        const double ePlusOne = ELASTIC_MODULUS + 1.0;

        double r, inv, ma, mb, mm, dvl, cosd, ud, effect;
        quint64 a, b;
        Vector3 coa, cob, cod, udv, uav, ubv, dir, vav, vbv;

        double* vels = new double[numberOfParticles * 3]();

        quint64 k = m_interactionStart * numberOfParticles - (m_interactionStart + 1) * m_interactionStart / 2;

        for (quint64 i = m_interactionStart; i < m_interactionEnd; ++i)
        {
            a = i * 3;
            for (quint64 j = i + 1; j < numberOfParticles; ++j, ++k)
            {
                b = j * 3;
                coa.set(coordinates, a);
                cob.set(coordinates, b);
                cod.set(cob - coa);
                r = cod.abs();
                if (r == 0.0)
                {
                    //qDebug() << "R=0" << a << b;
                    flags[k] = 1.0;
                    continue;
                }
                ma = masses[i];
                mb = masses[j];
                if (r < BOUNCY_THRESHOLD)
                {
                    if (flags[k] == 0.0)
                    {
                        dir.set(cod.normalized());
                        uav.set(velocities, a);
                        ubv.set(velocities, b);
                        udv.set(ubv - uav);
                        dvl = udv.abs();
                        if (dvl == 0.0)
                        {
                            //qDebug() << "DV=0" << a << b;
                            flags[k] = 1.0;
                            continue;
                        }
                        cosd = dir.dot(udv.normalized());
                        if (cosd > COS_MAX_THRESHOLD) {
                            ud = dvl;
                        } else if (cosd < COS_MIN_THRESHOLD) {
                            ud = - dvl;
                        } else {
                            ud = dvl * cosd;
                        }
                        mm = ma + mb;
                        effect = ePlusOne * ud / mm;
                        vav.set(dir * (   effect * mb));
                        vbv.set(dir * ( - effect * ma));
                        vels[a    ] += vav.x();
                        vels[a + 1] += vav.y();
                        vels[a + 2] += vav.z();
                        vels[b    ] += vbv.x();
                        vels[b + 1] += vbv.y();
                        vels[b + 2] += vbv.z();
                        flags[k] = 1.0;
                    }
                    inv = 1.0 / r;
                    effect = inv * inv * inv * repulsive;
                    vav.set(cod * (   effect * mb));
                    vbv.set(cod * ( - effect * ma));
                    vels[a    ] += vav.x();
                    vels[a + 1] += vav.y();
                    vels[a + 2] += vav.z();
                    vels[b    ] += vbv.x();
                    vels[b + 1] += vbv.y();
                    vels[b + 2] += vbv.z();
                } else {
                    flags[k] = 0.0;
                    inv = 1.0 / r;
                    effect = inv * inv * inv * timeG;
                    vav.set(cod * (   effect * mb));
                    vbv.set(cod * ( - effect * ma));
                    vels[a    ] += vav.x();
                    vels[a + 1] += vav.y();
                    vels[a + 2] += vav.z();
                    vels[b    ] += vbv.x();
                    vels[b + 1] += vbv.y();
                    vels[b + 2] += vbv.z();
                }
            }
        }
        bhs::interactionMutex.lock();
        quint64 end = numberOfParticles * 3;
        for (quint64 i = 0; i < end; ++i)
        {
            r = velocities[i];
            velocities[i] += vels[i];
            if ( ! std::isfinite(velocities[i]))
            {
                //qDebug() << "VEL" << i << vels[i];
                velocities[i] = r;
            }
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
