#pragma once

#include "engine/abstractnbodyengine.h"

/*
Inelastic collision

Velocities: ua, ub -> va, vb
Masses: ma, mb
Coefficient of Restitution: e

va = (e mb(ub - ua) + ma ua + mb ub) / (ma + mb)
vb = (e ma(ua - ub) + ma ua + mb ub) / (ma + mb)

When ua -> 0
ud = ub - ua
va = (e + 1)mb ud / (ma + mb)

Impulsive Force: ma va

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
        static const double CONTINUE_THRESHOLD = 1e-10;
        static const double BOUNDARY_THRESHOLD = 0.04;
        static const double COS_THRESHOLD = 1.0;
        static const double RESTITUTION_PLUS_ONE = 1.9;

        double* const coordinates = m_engine->coordinates();
        double* const velocities = m_engine->velocities();
        const double* const masses = m_engine->masses();
        double* const flags = m_engine->distances();

        const double timePerFrame = m_engine->timePerFrame();
        const double gravitationalConstant = m_engine->gravitationalConstant();
        const double timeG = timePerFrame * gravitationalConstant;
        const quint64 numberOfParticles = m_engine->numberOfParticle();

        double r, inv, ma, mb, mm, dvl, cosd, ud, effect;
        quint64 a, b;
        Vector3 coa, cob, udv, uav, ubv, dir, vav, vbv;

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
                udv.set(cob - coa);
                ma = masses[i];
                mb = masses[j];
                r = udv.abs();
                if (r <= CONTINUE_THRESHOLD)
                {
                    qDebug() << "continue r" << a << b;
                    continue;
                }
                if (r <= BOUNDARY_THRESHOLD)
                {
                    if (flags[k] == 0.0)
                    {
                        dir.set(udv.normalized());
                        uav.set(velocities, a);
                        ubv.set(velocities, b);
                        udv.set(ubv - uav);
                        dvl = udv.abs();
                        if (dvl <= CONTINUE_THRESHOLD)
                        {
                            qDebug() << "continue dvl" << a << b;
                            flags[k] = 1.0;
                            continue;
                        }
                        cosd = dir.dot(udv) / dvl;
                        //if ( ! std::isfinite(cosd))
                        //{
                        //    qDebug() << "cos" << a << b << cosd;
                        //    flags[k] = 1.0;
                        //    continue;
                        //}
                        if (cosd > COS_THRESHOLD) {
                            ud = dvl;
                        } else if (cosd < -COS_THRESHOLD) {
                            ud = -dvl;
                        } else {
                            ud = dvl * cosd;
                        }
                        mm = ma + mb;
                        effect = RESTITUTION_PLUS_ONE * ud / mm;
                        //if ( ! std::isfinite(effect))
                        //{
                        //    qDebug() << "force" << a << b << effect;
                        //    flags[k] = 1.0;
                        //    continue;
                        //}
                        vav.set(dir * ( effect * mb));
                        vbv.set(dir * (-effect * ma));
                        if (vav.isfinite() && vbv.isfinite()) {
                            vels[a    ] += vav.x();
                            vels[a + 1] += vav.y();
                            vels[a + 2] += vav.z();
                            vels[b    ] += vbv.x();
                            vels[b + 1] += vbv.y();
                            vels[b + 2] += vbv.z();
                        } else {
                            qDebug() << "CLL" << a << b << "vav" << vav.toString().c_str() << "vbv" << vbv.toString().c_str();
                        }
                        flags[k] = 1.0;
                    }
                } else {
                    flags[k] = 0.0;
                    inv = 1.0 / r;
                    effect = inv * inv * inv * timeG;
                    vav.set(udv * ( effect * mb));
                    vbv.set(udv * (-effect * ma));
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
