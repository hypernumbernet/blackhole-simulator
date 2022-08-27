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
        static const double MIN_THRESHOLD = 0.03;
        static const double BOUNDARY_THRESHOLD = 0.04;
        static const double COS_THRESHOLD = 0.999;
        static const double RESTITUTION = 0.98;

        double* const coordinates = m_engine->coordinates();
        double* const velocities = m_engine->velocities();
        const double* const masses = m_engine->masses();
        double* const flags = m_engine->distances();

        const double timePerFrame = m_engine->timePerFrame();
        //const double scaleInv = m_engine->scaleInv();
        const double gravitationalConstant = m_engine->gravitationalConstant();
        const double timeG = timePerFrame * gravitationalConstant;
        const quint64 numberOfParticles = m_engine->numberOfParticle();

        double r, inv, theta;
        double ma, mb, mm, dvl, cosd, ud, forseS;
        quint64 a, b;
        Vector3 coa, cob, udv, uav, ubv, dir, uao, ubo, forseV;

        double* vels = new double[numberOfParticles * 3]();

        quint64 k = m_interactionStart * numberOfParticles - (m_interactionStart + 1) * m_interactionStart / 2;

        for (quint64 i = m_interactionStart; i < m_interactionEnd; ++i)
        {
            a = i * 3;
            for (quint64 j = i + 1; j < numberOfParticles; ++j)
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
                    continue;
                }
                if (r > MIN_THRESHOLD)
                {
                    inv = 1.0 / r;
                    theta = inv * inv * inv * timeG;
                    dir.set(udv * theta);
                    vels[a    ] += dir.x() * mb;
                    vels[a + 1] += dir.y() * mb;
                    vels[a + 2] += dir.z() * mb;
                    vels[b    ] -= dir.x() * ma;
                    vels[b + 1] -= dir.y() * ma;
                    vels[b + 2] -= dir.z() * ma;
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
                            continue;
                        cosd = dir.dot(udv) / dvl;
                        if (abs(cosd) >= COS_THRESHOLD) {
                            ud = dvl;
                        } else {
                            ud = dvl * cosd;
                        }
                        mm = ma + mb;
                        forseS = ma * (RESTITUTION + 1.0) * mb * ud / mm;
                        forseV.set(dir * forseS);
                        uao.set(forseV / ma);
                        ubo.set(forseV / mb);
                        qDebug() << "uao: " << uao.toString().c_str();
                        qDebug() << "ubo: " << ubo.toString().c_str();
                        vels[a    ] += uao.x();
                        vels[a + 1] += uao.y();
                        vels[a + 2] += uao.z();
                        vels[b    ] += ubo.x();
                        vels[b + 1] += ubo.y();
                        vels[b + 2] += ubo.z();
                        flags[k] = 1.0;
                    }
                } else {
                    flags[k] = 0.0;
                }
                ++k;
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
