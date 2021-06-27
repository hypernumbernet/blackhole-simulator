#include "initializer3d3s.h"

void Initializer3D3S::initRandamCube()
{
    m_engine->changeModelScale(1.0e-11f);

    const quint64 num = m_engine->numberOfParticle();
    float* const coordinates = m_engine->coordinates();
    float* const velocities = m_engine->velocities();

    float* const masses = m_engine->masses();
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
        coordinates[i] = bhs::rand0center1maxf() * 1.0e+11f;
    }

    bhs::Vector3<float> axis;
    float angle;
    quint64 i4;
    for (quint64 i = 0; i < num; ++i)
    {
        axis.set(bhs::randf(), bhs::randf(), bhs::randf());
        axis.Normalize();
        angle = bhs::rand0center1maxf() * 1.5e+4f * AbstractNBodyEngine::VANGLE;
        auto vq = bhs::Quaternion<float>::Exp(axis * angle);
        i4 = i * 4;
        velocities[i4    ] = vq.i0;
        velocities[i4 + 1] = vq.i1;
        velocities[i4 + 2] = vq.i2;
        velocities[i4 + 3] = vq.i3;
    }
}

void Initializer3D3S::initRandamSphere(const float rate)
{
    m_engine->changeModelScale(1.0e-13f);

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

        cood *= 1.0e+13f;
        coordinates[i3    ] = cood.x;
        coordinates[i3 + 1] = cood.y;
        coordinates[i3 + 2] = cood.z;
    }

    bhs::Vector3<float> axis;
    float angle;
    quint64 i4;
    for (quint64 i = 0; i < num; ++i)
    {
        axis.set(bhs::randf(), bhs::randf(), bhs::randf());
        axis.Normalize();
        angle = bhs::rand0center1maxf() * 1.5e+4f * AbstractNBodyEngine::VANGLE;
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
    auto q = fromDirectionAndSpeed(0.0f, 1.0f, 0.0f, 29780.0f);
    embedQuaternionToArray(q, velocities, 4);
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
    auto q = fromDirectionAndSpeed(0.0f, 1.0f, 0.0f, 29780.0f);
    embedQuaternionToArray(q, velocities, 0);

    masses[1] = 1.9891e+30f;
    coordinates[3] = 1.495978e+11f;
    coordinates[4] = 0.0f;
    coordinates[5] = 0.0f;
    velocities[4] = 1.0f;
    velocities[5] = 0.0f;
    velocities[6] = 0.0f;
    velocities[7] = 0.0f;
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
    velocities[0] = 1.0f;
    velocities[1] = 0.0f;
    velocities[2] = 0.0f;
    velocities[3] = 0.0f;

    masses[1] = 7.347673e+22f;
    coordinates[3] = 3.844e+8f;
    coordinates[4] = 0.0f;
    coordinates[5] = 0.0f;
    auto q = fromDirectionAndSpeed(0.0f, 1.0f, 0.0f, 1022.0f);
    embedQuaternionToArray(q, velocities, 4);
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
    velocities[0] = 1.0f;
    velocities[1] = 0.0f;
    velocities[2] = 0.0f;
    velocities[3] = 0.0f;

    masses[1] = 5.972e+24f;
    coordinates[3] = 1.495978e+11f;
    coordinates[4] = 0.0f;
    coordinates[5] = 0.0f;
    auto earth = fromDirectionAndSpeed(0.0f, 0.0f, 1.0f, 29780.0f);
    embedQuaternionToArray(earth, velocities, 4);

    masses[2] = 4.869e+24f;
    coordinates[6] = 1.0820893e+11f;
    coordinates[7] = 0.0f;
    coordinates[8] = 0.0f;
    auto venus = fromDirectionAndSpeed(0.0f, 0.0f, 1.0f, 35021.4f);
    embedQuaternionToArray(venus, velocities, 8);
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
    velocities[0] = 1.0f;
    velocities[1] = 0.0f;
    velocities[2] = 0.0f;
    velocities[3] = 0.0f;

    masses[1] = 2.0e+10f;
    coordinates[3] = 0.1f;
    coordinates[4] = 0.2f;
    coordinates[5] = 0.3f;
    velocities[4] = 1.0f;
    velocities[5] = 0.0f;
    velocities[6] = 0.0f;
    velocities[7] = 0.0f;
}
