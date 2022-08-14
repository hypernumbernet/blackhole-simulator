#pragma once

#include <QObject>

#include "engine/abstractnbodyengine.h"

using namespace hnn;

template <typename T>
class G4D3DCalculation
{
public:
    G4D3DCalculation(AbstractNBodyEngine<T>* const engine, const int threadNumber)
        : m_engine(engine)
        , m_timeProgresStart(engine->timeProgressRanges().at(threadNumber).start)
        , m_timeProgresEnd(engine->timeProgressRanges().at(threadNumber).end)
        , m_interactionStart(engine->interactionRanges().at(threadNumber).start)
        , m_interactionEnd(engine->interactionRanges().at(threadNumber).end)
    {
    }

    inline void calculateTimeProgress() const
    {
        T* const coordinates = m_engine->coordinates();
        T* const locations = m_engine->locations();
        const T* const velocities = m_engine->velocities();

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            quint64 i3 = i * 3;
            quint64 i4 = i * 4;
            auto vq = Quaternion<T>::exp({velocities, i3});
            auto lq = Quaternion<T>(locations, i4);
            lq = vq * lq;
            //lq = lq * vq;
            bhs::embedQuaternionToArray<T>(lq, locations, i4);
            auto ln = lq.lnV3();
            bhs::embedVector3ToArray<T>(ln, coordinates, i3);
        }
    }

    inline void calculateInteraction() const
    {
        const T* const coordinates = m_engine->coordinates();
        T* const velocities = m_engine->velocities();
        T* const distanceInv = m_engine->distanceInv();
        const quint64 numberOfParticles = m_engine->numberOfParticle();
        const T timePerFrame = m_engine->timePerFrame();
        const T gravitationalConstant = m_engine->gravitationalConstant();
        const T timeG = timePerFrame * gravitationalConstant;
        //const T boundaryToInvalidate = AbstractNBodyEngine<T>::BOUNDARY_TO_INVALIDATE;

        quint64 k = m_interactionStart * numberOfParticles - (m_interactionStart + 1) * m_interactionStart / 2;

        T d1, d2, d3, r, theta;
        quint64 i3, j3;

        Quaternion<T>* vels = new Quaternion<T>[numberOfParticles]();
        for (quint64 i = 0; i < numberOfParticles; ++i)
        {
            vels[i].set(1.0, 0.0, 0.0, 0.0);
        }

        for (quint64 i = m_interactionStart; i < m_interactionEnd; ++i)
        {
            i3 = i * 3;
            for (quint64 j = i + 1; j < numberOfParticles; ++j)
            {
                j3 = j * 3;

                d1 = coordinates[j3    ] - coordinates[i3    ];
                d2 = coordinates[j3 + 1] - coordinates[i3 + 1];
                d3 = coordinates[j3 + 2] - coordinates[i3 + 2];

                r = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                //if (r <= boundaryToInvalidate)
                //    continue;

                theta = distanceInv[k];
                distanceInv[k] = 1.0 / r;
                theta -= distanceInv[k];

                if (theta == theta)
                {
                    theta = fabs(theta);
                    theta *= timeG;
                    d1 *= theta;
                    d2 *= theta;
                    d3 *= theta;
                    vels[i] *= Quaternion<T>::exp(d1, d2, d3);
                    vels[j] *= Quaternion<T>::exp(-d1, -d2, -d3);
                } else {
                    distanceInv[k] = std::numeric_limits<T>::max();
                }
                ++k;
            }
        }

        bhs::interactionMutex.lock();
        for (quint64 i = 0; i < numberOfParticles; ++i)
        {
            i3 = i * 3;
            auto speed = vels[i].lnV3();
            velocities[i3    ] += speed.x();
            velocities[i3 + 1] += speed.y();
            velocities[i3 + 2] += speed.z();
        }
        bhs::interactionMutex.unlock();

        delete[] vels;
    }

private:
    AbstractNBodyEngine<T>* const m_engine;

    const quint64 m_timeProgresStart;
    const quint64 m_timeProgresEnd;
    const quint64 m_interactionStart;
    const quint64 m_interactionEnd;
};
