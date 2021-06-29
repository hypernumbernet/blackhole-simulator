#pragma once

#include "abstractenginecoredouble.h"
#include "quaternion.h"

using namespace bhs;

class G3D4DMassDiffCoreDouble : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    explicit G3D4DMassDiffCoreDouble(AbstractNBodyEngine<double>* const engine, QObject* parent = nullptr)
        : AbstractEngineCoreDouble(engine, parent)
        , m_cinv(1.0f / AbstractNBodyEngine<double>::SPEED_OF_LIGHT)

        // Half the circumference is assumed to be the speed of light.
        , m_vangle(AbstractNBodyEngine<double>::PI * m_cinv)

        , m_vangle_inv(1.0f / m_vangle)
    {
    }

public slots:
    void calculateTimeProgress(int threadNumber) const
    {
        quint64 start = m_engine->timeProgressRanges().at(threadNumber).start;
        quint64 end = m_engine->timeProgressRanges().at(threadNumber).end;

        for (quint64 i = start; i < end; ++i)
        {
            auto vq = Quaternion<double>(m_velocities, i * 4);
            vq.Normalize();
            auto vv3 = vq.LnV3();

            auto to_add = vv3 * m_vangle_inv * m_timePerFrame;

            quint64 j = i * 3;
            m_coordinates[j] += to_add.x; ++j;
            m_coordinates[j] += to_add.y; ++j;
            m_coordinates[j] += to_add.z;
        }
        resultReady();
    }

    void calculateInteraction(int threadNumber) const
    {
        const quint64 start = m_engine->interactionRanges().at(threadNumber).start;
        const quint64 end = m_engine->interactionRanges().at(threadNumber).end;
        const double vangle_half = m_vangle * 0.5f;
        const double time_g = m_timePerFrame * AbstractNBodyEngine<double>::GRAVITATIONAL_CONSTANT;

        double d1, d2, d3, distance, inv, theta;
        quint64 k = 0, a, b;

        double* vels = new double[m_numberOfParticles * 4];
        for (quint64 i = 0; i < m_numberOfParticles; ++i) {
            auto i4 = i * 4;
            vels[i4    ] = 1.0f;
            vels[i4 + 1] = 0.0f;
            vels[i4 + 2] = 0.0f;
            vels[i4 + 3] = 0.0f;
        }

        for (quint64 i = start; i < end; ++i)
        {
            for (quint64 j = i + 1; j < m_numberOfParticles; ++j)
            {
                a = i * 3;
                b = j * 3;

                d1 = m_coordinates[a    ] - m_coordinates[b    ];
                d2 = m_coordinates[a + 1] - m_coordinates[b + 1];
                d3 = m_coordinates[a + 2] - m_coordinates[b + 2];
                distance = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                if (distance <= 0.0f) {
                    continue;
                }
                inv = 1.0f / distance;
                theta = inv * inv * time_g;

                d1 *= inv;
                d2 *= inv;
                d3 *= inv;

                auto rota = Quaternion<double>(0.0, d1, d2, d3);
                auto rotb = Quaternion<double>(0.0, d1, d2, d3);
                rota.MakeRotation(-theta * m_masses[j] * vangle_half);
                rotb.MakeRotation(theta * m_masses[i] * vangle_half);

                a = i * 4;
                b = j * 4;
                auto va = Quaternion<double>(vels, a);
                auto vb = Quaternion<double>(vels, b);

                auto rotatedA = va.Rot8(rota);
                auto rotatedB = vb.Rot8(rotb);

                vels[a    ] = rotatedA.i0;
                vels[a + 1] = rotatedA.i1;
                vels[a + 2] = rotatedA.i2;
                vels[a + 3] = rotatedA.i3;

                vels[b    ] = rotatedB.i0;
                vels[b + 1] = rotatedB.i1;
                vels[b + 2] = rotatedB.i2;
                vels[b + 3] = rotatedB.i3;

                ++k;
            }
        }

        interactionMutex.lock();
        for (quint64 i = 0; i < m_numberOfParticles; ++i) {
            auto i4 = i * 4;
            auto vq = Quaternion<double>(m_velocities, i4);
            auto vp = Quaternion<double>(vels, i4);
            vq *= vp;

            m_velocities[i4    ] = vq.i0;
            m_velocities[i4 + 1] = vq.i1;
            m_velocities[i4 + 2] = vq.i2;
            m_velocities[i4 + 3] = vq.i3;
        }
        interactionMutex.unlock();

        delete[] vels;

        resultReady();
    }

private:
    const double m_cinv;
    const double m_vangle;
    const double m_vangle_inv;
};