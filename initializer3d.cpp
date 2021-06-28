#include "initializer3d.h"

void Initializer3D::initRandamCube()
{
    m_engine->changeModelScale(1.0 / m_sim.scale);

    quint64 num = m_engine->numberOfParticle();
    float* masses = m_engine->masses();
    float* coordinates = m_engine->coordinates();
    float* velocities = m_engine->velocities();

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
        coordinates[i] = bhs::rand0center1maxf() * (float)m_sim.scale;
    }
    for (quint64 i = 0; i < num * 3; ++i)
    {
        velocities[i] = bhs::rand0center1maxf() * 1.5e+4f;
    }

//    masses[0] = 2.0e+30f;
//    coordinates[0] = 0.0f;
//    coordinates[1] = 0.0f;
//    coordinates[2] = 0.0f;
}

void Initializer3D::initRandamSphere(const float rate)
{
    m_engine->changeModelScale(1.0 / m_sim.scale);

    quint64 num = m_engine->numberOfParticle();
    float* masses = m_engine->masses();
    float* coordinates = m_engine->coordinates();
    float* velocities = m_engine->velocities();

    if (m_sim.massRandom) {
        for (quint64 i = 0; i < num; ++i) {
            masses[i] = bhs::randf() * m_sim.massAvg * 2.0f;
        }
    } else {
        for (quint64 i = 0; i < num; ++i) {
            masses[i] = m_sim.massAvg;
        }
    }

    quint64 i3;
    bhs::Vector3<float> cood;
    for (quint64 i = 0; i < num; ++i)
    {
        i3 = i * 3;

        do {
            cood.set(
                bhs::rand0center1maxf(),
                bhs::rand0center1maxf(),
                bhs::rand0center1maxf());
        }
        while (cood.Norm() > 1.0f || cood.Norm() < rate);

        cood *= (float)m_sim.scale;
        coordinates[i3    ] = cood.x;
        coordinates[i3 + 1] = cood.y;
        coordinates[i3 + 2] = cood.z;
    }

    for (quint64 i = 0; i < num * 3; ++i)
    {
        velocities[i] = bhs::randf() * 3.0e+4f - 1.5e+4f;
    }
}

void Initializer3D::initSunEarth()
{
    m_engine->changeModelScale(1.0e-11f);

    float* masses = m_engine->masses();
    float* coordinates = m_engine->coordinates();
    float* velocities = m_engine->velocities();

    masses[0] = 1.9891e+30f;
    coordinates[0] = coordinates[1] = coordinates[2] = 0.0f;
    velocities[0] = velocities[1] = velocities[2] = 0.0f;

    masses[1] = 5.972e+24f;
    coordinates[3] = 1.495978e+11f;
    coordinates[4] = 0.0f;
    coordinates[5] = 0.0f;
    velocities[3] = 0.0f;
    velocities[4] = 29780.0f;
    velocities[5] = 0.0f;
}

void Initializer3D::initEarthSun()
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

void Initializer3D::initEarthMoon()
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

void Initializer3D::initSunEarthVenus()
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

void Initializer3D::initTestSamePosition()
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
