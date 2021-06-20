#include "initializer3d.h"

Initializer3D::Initializer3D()
{

}

void Initializer3D::initParticlesRandam(AbstractNBodyEngine* const engine)
{
    engine->changeModelScale(1.0e-3f);

    quint64 num = engine->numberOfParticle();
    float* masses = engine->masses();
    float* coordinates = engine->coordinates();
    float* velocities = engine->velocities();

    for (quint64 i = 0; i < num; ++i)
    {
        masses[i] = randf() * 2.0e+2f - 1.0e+2f;
        //masses[i] = 2.0e+2f;
    }
    for (quint64 i = 0; i < num * 3; ++i)
    {
        coordinates[i] = randf() * 2.0e+3f - 1.0e+3f;
    }
    for (quint64 i = 0; i < num * 3; ++i)
    {
        velocities[i] = randf() * 2.0e-7f - 1.0e-7f;
        //velocities[i] = 0.0f;
    }

    masses[0] = 2.0e+5f;
//    coordinates[0] = 0.0f;
//    coordinates[1] = 0.0f;
//    coordinates[2] = 0.0f;

}

void Initializer3D::initSunEarth(AbstractNBodyEngine* const engine)
{
    engine->changeModelScale(1.0e-11f);

    float* masses = engine->masses();
    float* coordinates = engine->coordinates();
    float* velocities = engine->velocities();

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

void Initializer3D::initEarthSun(AbstractNBodyEngine* const engine)
{
    engine->changeModelScale(1.0e-11f);

    float* masses = engine->masses();
    float* coordinates = engine->coordinates();
    float* velocities = engine->velocities();

    masses[0] = 5.972e+24f;
    coordinates[0] = 0.0f;
    coordinates[1] = 0.0f;
    coordinates[2] = 0.0f;
    velocities[0] = 0.0f;
    velocities[1] = 29780.0f;
    velocities[2] = 0.0f;

    masses[1] = 1.9891e+30f;
    coordinates[3] = 1.495978e+11f;
    coordinates[4] = 0.0f;
    coordinates[5] = 0.0f;
    velocities[3] = 0.0f;
    velocities[4] = 0.0f;
    velocities[5] = 0.0f;
}

void Initializer3D::initEarthMoon(AbstractNBodyEngine* const engine)
{
    engine->changeModelScale(0.25e-8f);

    float* masses = engine->masses();
    float* coordinates = engine->coordinates();
    float* velocities = engine->velocities();

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

void Initializer3D::initSunEarthVenus(AbstractNBodyEngine* const engine)
{
    engine->changeModelScale(1.0e-11f);

    float* masses = engine->masses();
    float* coordinates = engine->coordinates();
    float* velocities = engine->velocities();

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

void Initializer3D::initTestSamePosition(AbstractNBodyEngine* const engine)
{
    engine->changeModelScale(1.0f);

    float* masses = engine->masses();
    float* coordinates = engine->coordinates();
    float* velocities = engine->velocities();

    masses[0] = 1.0e+10f;
    coordinates[0] = 1.0f;
    coordinates[1] = 2.0f;
    coordinates[2] = 3.0f;
    velocities[0] = 0.0f;
    velocities[1] = 0.0f;
    velocities[2] = 0.0f;

    masses[1] = 2.0e+10f;
    coordinates[3] = 1.0f;
    coordinates[4] = 2.0f;
    coordinates[5] = 3.0f;
    velocities[3] = 0.0f;
    velocities[4] = 0.0f;
    velocities[5] = 0.0f;
}
