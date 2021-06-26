#include "g3svmassdiffcore.h"

using namespace bhs;

// Gravity 3D-Coordinate 3S-Velocity Mass Differential N-Body Engine

G3SVMassDiffCore::G3SVMassDiffCore(AbstractNBodyEngine* const engine, QObject* parent)
    : AbstractEngineCore(engine, parent)
{

}

void G3SVMassDiffCore::calculateTimeProgress(int threadNumber) const
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

void G3SVMassDiffCore::calculateInteraction(int threadNumber) const
{
    quint64 start = m_engine->interactionRanges().at(threadNumber).start;
    quint64 end = m_engine->interactionRanges().at(threadNumber).end;

    float d1, d2, d3, distance, inv, theta;
    quint64 k = 0, a, b;
    float cinv = 1.0f / AbstractNBodyEngine::SPEED_OF_LIGHT;

    // Half the circumference is assumed to be the speed of light.
    float velangle = AbstractNBodyEngine::PI * cinv;

    float vangle_half = velangle * 0.5f;
    float vangle_inv = 1.0f / velangle;
    float time_g = m_timePerFrame * AbstractNBodyEngine::GRAVITATIONAL_CONSTANT;

    bhs::interactionMutex.lock();
//    float* vels = new float[m_numberOfParticles * 3];
//    for (quint64 i = 0; i < m_numberOfParticles * 3; ++i) {
//        vels[i] = m_velocities[i];
//    }

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

            auto rota = Quaternion<float>(0.0, d1, d2, d3);
            auto rotb = Quaternion<float>(0.0, d1, d2, d3);
            rota.MakeRotation(-theta * m_masses[j] * vangle_half);
            rotb.MakeRotation(theta * m_masses[i] * vangle_half);

            auto vva = Vector3<float>(m_velocities, a);
            auto vvb = Vector3<float>(m_velocities, b);
            auto va = Quaternion<float>::Exp(vva * velangle);
            auto vb = Quaternion<float>::Exp(vvb * velangle);

            auto rotatedA = va.Rot8(rota);
            auto rotatedB = vb.Rot8(rotb);

            auto v3a = rotatedA.LnV3();
            auto v3b = rotatedB.LnV3();
            m_velocities[a    ] = v3a.x * vangle_inv;
            m_velocities[a + 1] = v3a.y * vangle_inv;
            m_velocities[a + 2] = v3a.z * vangle_inv;
            m_velocities[b    ] = v3b.x * vangle_inv;
            m_velocities[b + 1] = v3b.y * vangle_inv;
            m_velocities[b + 2] = v3b.z * vangle_inv;

            ++k;
        }
    }

//    bhs::interactionMutex.lock();
//    for (quint64 i = 0; i < m_numberOfParticles * 3; ++i) {
//        m_velocities[i] = vels[i];
//    }
    bhs::interactionMutex.unlock();

//    delete[] vels;

    resultReady();
}
