#pragma once

#include "engine/AbstractNBodyEngine.h"
#include "InitializerUniverse1.h"

using namespace hnn;

class CalculationUniverse1
{
public:
    CalculationUniverse1(AbstractNBodyEngine<double>* const engine, const int threadNumber)
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
        const double angleToCoord = InitializerUniverse1<double>::ZOOM / PI;

        quint64 i3, i4;
        Quaternion vq, lq;
        Vector3 vv, lv;
        double re, angle;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            i3 = i * 3;
            i4 = i * 4;

            re = velocities[i4];
            if (re > 1.)
            {
                re = 1.;
            }
            else if (re < - 1.)
            {
                re = - 1.;
            }
            angle = acos(re);
            vv.set(velocities[i4 + 1], velocities[i4 + 2], velocities[i4 + 3]);
            vv.normalize();
            lq.set(locations, i4);
            lq.rotMove(vv, angle * timePerFrame);
            bhs::embedQuaternionToArray<double>(lq, locations, i4);
            lv.set(lq.lnV3() * angleToCoord);
            bhs::embedVector3ToArray<double>(lv, coordinates, i3);
        }
    }

    void calculateInteraction() const
    {
        const double* const locations = m_engine->locations();
        double* const velocities = m_engine->velocities();
        const double* const masses = m_engine->masses();
        double* const distances = m_engine->distances();
        const quint64 numberOfParticles = m_engine->numberOfParticle();
        const double timePerFrame = m_engine->timePerFrame();
        const double gravitationalConstant = m_engine->gravitationalConstant();
        //const double coordToAngle = PI / InitializerUniverse1<double>::ZOOM;
        const double timeG = timePerFrame * gravitationalConstant / PI;
        //const double boundaryToInvalidate = AbstractNBodyEngine<double>::BOUNDARY_TO_INVALIDATE;

        quint64 k = m_interactionStart * numberOfParticles - (m_interactionStart + 1) * m_interactionStart / 2;

        double dot, r, inv, theta, ma, mb;
        quint64 i, j, i4, j4;
        Quaternion qa, qb, va, vb;
        Vector3 direction;

        for (i = m_interactionStart; i < m_interactionEnd; ++i)
        {
            i4 = i * 4;

            for (j = i + 1; j < numberOfParticles; ++j)
            {
                j4 = j * 4;

                qa.set(locations, i4);
                qb.set(locations, j4);

                va.set(velocities, i4);
                vb.set(velocities, j4);

                dot = qa.dot(qb);
                if (dot >= 1.)
                {
                    r = std::numeric_limits<double>::min();
                    //continue;
                }
                else if (dot < - 1.)
                {
                    r = - 1.;
                }
                else
                {
                    r = acos(dot);
                }

                theta = distances[k];
                inv = 1. / r;
                theta -= inv;
                theta = fabs(theta);
                theta *= timeG;
                ma = masses[i];
                mb = masses[j];

                direction = qa.cross7V3(qb);
                direction.normalize();

                va.rotMove(direction, - theta * mb);
                vb.rotMove(direction,   theta * ma);

                bhs::interactionMutex.lock();
                bhs::embedQuaternionToArray(va, velocities, i4);
                bhs::embedQuaternionToArray(vb, velocities, j4);
                distances[k] = inv;
                bhs::interactionMutex.unlock();

                ++k;
            }
        }
    }

private:
    AbstractNBodyEngine<double>* const m_engine;

    const quint64 m_timeProgresStart;
    const quint64 m_timeProgresEnd;
    const quint64 m_interactionStart;
    const quint64 m_interactionEnd;
};
