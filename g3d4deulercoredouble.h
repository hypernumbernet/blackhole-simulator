#pragma once

#include "abstractenginecoredouble.h"
#include "quaternion.h"

using namespace bhs;

class G3D4DEulerCoreDouble : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    explicit G3D4DEulerCoreDouble(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_cinv(1.0 / AbstractNBodyEngine<double>::SPEED_OF_LIGHT)

        // Half the circumference is assumed to be the speed of light.
        , m_vangle(AbstractNBodyEngine<double>::PI * m_cinv)

        , m_vangle_inv(1.0 / m_vangle)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new G3D4DEulerCoreDouble(engine, threadNumber);
    }

public slots:
    void calculateTimeProgress() const
    {
        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
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

    void calculateInteraction() const
    {
        const double vangle_half = m_vangle * 0.5;
        const double time_g = m_timePerFrame * m_engine->m_gravitationalConstant;

        double d1, d2, d3, r, inv, theta;
        quint64 a, b;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            a = i * 3;

            double total_x = 0.0;
            double total_y = 0.0;
            double total_z = 0.0;

            for (quint64 j = 0; j < m_numberOfParticles; ++j)
            {
                if (i == j)
                    continue;

                b = j * 3;

                d1 = m_coordinates[b    ] - m_coordinates[a    ];
                d2 = m_coordinates[b + 1] - m_coordinates[a + 1];
                d3 = m_coordinates[b + 2] - m_coordinates[a + 2];

                r = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                if (r <= 0.0)
                    continue;

                inv = 1.0 / r;
                theta = inv * inv * inv * time_g * m_masses[j];

                d1 *= theta;
                d2 *= theta;
                d3 *= theta;

                total_x += d1;
                total_y += d2;
                total_z += d3;
            }
            a = i * 4;

            r = sqrt(total_x * total_x + total_y * total_y + total_z * total_z);
            if (r <= 0.0)
                continue;

            total_x /= r;
            total_y /= r;
            total_z /= r;
            auto acc = Quaternion<double>::MakeRotation(total_x, total_y, total_z, r * vangle_half);

            auto va = Quaternion<double>(m_velocities, a);

            va.Rotate8(acc);

            m_velocities[a    ] = va.i0;
            m_velocities[a + 1] = va.i1;
            m_velocities[a + 2] = va.i2;
            m_velocities[a + 3] = va.i3;
        }

        resultReady();
    }

private:
    const double m_cinv;
    const double m_vangle;
    const double m_vangle_inv;
};
