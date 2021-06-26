#include "initializer3d3s.h"

void Initializer3D3S::initParticlesRandam()
{
    m_engine->changeModelScale(1.0e-11f);

    const quint64 num = m_engine->numberOfParticle();
    float* const masses = m_engine->masses();
    float* const coordinates = m_engine->coordinates();
    float* const velocities = m_engine->velocities();

    auto axis = bhs::Vector3<float>();
    float angle;
    quint64 i4;

    const float vangle = AbstractNBodyEngine::PI / AbstractNBodyEngine::SPEED_OF_LIGHT;

    if (m_sim.massRandom) {
        for (quint64 i = 0; i < num; ++i) {
            masses[i] = bhs::randf() * m_sim.massAvg * 2.0f;
        }
    } else {
        for (quint64 i = 0; i < num; ++i) {
            masses[i] = m_sim.massAvg;
        }
    }
    for (quint64 i = 0; i < num * 3; ++i)
    {
        coordinates[i] = bhs::randf() * 2.0e+11f - 1.0e+11f;
    }
    for (quint64 i = 0; i < num; ++i)
    {
        axis.set(bhs::randf(), bhs::randf(), bhs::randf());
        axis.Normalize();

        angle = (bhs::randf() * 3.0e+4f - 1.5e+4f) * vangle;

        auto vq = bhs::Quaternion<float>::Exp(axis * angle);

        i4 = i * 4;
        velocities[i4    ] = vq.i0;
        velocities[i4 + 1] = vq.i1;
        velocities[i4 + 2] = vq.i2;
        velocities[i4 + 3] = vq.i3;
    }
}

void Initializer3D3S::initSunEarth()
{
    m_engine->changeModelScale(1.0e-11f);

    float* masses = m_engine->masses();
    float* coordinates = m_engine->coordinates();
    float* velocities = m_engine->velocities();

    masses[0] = 1.9891e+30f;
    coordinates[0] = coordinates[1] = coordinates[2] = 0.0f;
    velocities[0] = 1.0f;
    velocities[1] = 0.0f;
    velocities[2] = 0.0f;
    velocities[3] = 0.0f;

    masses[1] = 5.972e+24f;
    coordinates[3] = 1.495978e+11f;
    coordinates[4] = 0.0f;
    coordinates[5] = 0.0f;

    bhs::Vector3<float> axis(0.0f, 1.0f, 0.0f);
    const float vangle = AbstractNBodyEngine::PI / AbstractNBodyEngine::SPEED_OF_LIGHT;
    auto angle = 29780.0f * vangle;
    auto vq = bhs::Quaternion<float>::Exp(axis * angle);

    velocities[4] = vq.i0;
    velocities[5] = vq.i1;
    velocities[6] = vq.i2;
    velocities[7] = vq.i3;
}

void Initializer3D3S::initEarthSun()
{
    m_engine->changeModelScale(1.0e-11f);

    float* masses = m_engine->masses();
    float* coordinates = m_engine->coordinates();
    float* velocities = m_engine->velocities();

    masses[0] = 5.972e+24f;
    coordinates[0] = 0.0f;
    coordinates[1] = 0.0f;
    coordinates[2] = 0.0f;
    velocities[0] = 0.0f;
    velocities[1] = 0.0f;
    velocities[2] = 29780.0f;

    masses[1] = 1.9891e+30f;
    coordinates[3] = 1.495978e+11f;
    coordinates[4] = 0.0f;
    coordinates[5] = 0.0f;
    velocities[3] = 0.0f;
    velocities[4] = 0.0f;
    velocities[5] = 0.0f;
}

void Initializer3D3S::initEarthMoon()
{
    m_engine->changeModelScale(0.25e-8f);

    float* masses = m_engine->masses();
    float* coordinates = m_engine->coordinates();
    float* velocities = m_engine->velocities();

    masses[0] = 5.972e+24f;
    coordinates[0] = 0.0f;
    coordinates[1] = 0.0f;
    coordinates[2] = 0.0f;
    velocities[0] = 0.0f;
    velocities[1] = 0.0f;
    velocities[2] = 0.0f;

    masses[1] = 7.347673e+22f;
    coordinates[3] = 3.844e+8f;
    coordinates[4] = 0.0f;
    coordinates[5] = 0.0f;
    velocities[3] = 0.0f;
    velocities[4] = 1022.0f;
    velocities[5] = 0.0f;
}

void Initializer3D3S::initSunEarthVenus()
{
    m_engine->changeModelScale(1.0e-11f);

    float* masses = m_engine->masses();
    float* coordinates = m_engine->coordinates();
    float* velocities = m_engine->velocities();

    masses[0] = 1.9891e+30f;
    coordinates[0] = 0.0f;
    coordinates[1] = 0.0f;
    coordinates[2] = 0.0f;
    velocities[0] = 0.0f;
    velocities[1] = 0.0f;
    velocities[2] = 0.0f;

    masses[1] = 5.972e+24f;
    coordinates[3] = 1.495978e+11f;
    coordinates[4] = 0.0f;
    coordinates[5] = 0.0f;
    velocities[3] = 0.0f;
    velocities[4] = 0.0f;
    velocities[5] = 29780.0f;

    masses[2] = 4.869e+24f;
    coordinates[6] = 1.0820893e+11f;
    coordinates[7] = 0.0f;
    coordinates[8] = 0.0f;
    velocities[6] = 0.0f;
    velocities[7] = 0.0f;
    velocities[8] = 35021.4f;
}

void Initializer3D3S::initTestSamePosition()
{
    m_engine->changeModelScale(1.0f);

    float* masses = m_engine->masses();
    float* coordinates = m_engine->coordinates();
    float* velocities = m_engine->velocities();

    masses[0] = 1.0e+10f;
    coordinates[0] = 0.1f;
    coordinates[1] = 0.2f;
    coordinates[2] = 0.3f;
    velocities[0] = 0.0f;
    velocities[1] = 0.0f;
    velocities[2] = 0.0f;

    masses[1] = 2.0e+10f;
    coordinates[3] = 0.1f;
    coordinates[4] = 0.2f;
    coordinates[5] = 0.3f;
    velocities[3] = 0.0f;
    velocities[4] = 0.0f;
    velocities[5] = 0.0f;
}
