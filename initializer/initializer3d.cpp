#include "initializer3d.h"

//####################################################################
//
// To avoid float overflow and(or) speed up the calculations.
//
// Convert
//
// 10^10 (m) = 10000000000 (m) = 1 (length here)
// 10^30 (kg) = 1000...00 (kg) = 1 (mass here)
//
// g constant : -10-10-10+30 = 0 (not change)
//
// Or you should choose the right scale for your calculations like this.
//
//####################################################################

template class Initializer3D<float>;
template class Initializer3D<double>;

template <typename T>
void Initializer3D<T>::initRandamCube()
{
    Correct correct(m_sim.scale);
    m_engine->changeModelScale(correct.m);

    quint64 num = m_engine->numberOfParticle();
    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    if (m_sim.massRandom)
    {
        for (quint64 i = 0; i < num; ++i)
        {
            masses[i] = (T)(rand0to1() * m_sim.massAvg * 2.0 * correct.kg);
        }
    } else {
        for (quint64 i = 0; i < num; ++i)
        {
            masses[i] = m_sim.massAvg;
        }
    }
    for (quint64 i = 0; i < num * 3; ++i)
    {
        coordinates[i] = (T)(bhs::rand0center1max() * m_sim.scale * correct.m);
    }
    for (quint64 i = 0; i < num * 3; ++i)
    {
        velocities[i] = (T)(bhs::rand0center1max() * 1.5e+4 * correct.m);
    }
}

template <typename T>
void Initializer3D<T>::initRandamSphere(const double rate)
{
    Correct correct(m_sim.scale);
    m_engine->changeModelScale(correct.m);

    quint64 num = m_engine->numberOfParticle();
    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    if (m_sim.massRandom)
    {
        for (quint64 i = 0; i < num; ++i)
        {
            masses[i] = (T)(rand0to1() * m_sim.massAvg * 2.0 * correct.kg);
        }
    } else {
        for (quint64 i = 0; i < num; ++i)
        {
            masses[i] = m_sim.massAvg * correct.kg;
        }
    }

    quint64 i3;
    bhs::Vector3<T> coord;
    for (quint64 i = 0; i < num; ++i)
    {
        i3 = i * 3;
        do
        {
            coord.set(
                bhs::rand0center1max(),
                bhs::rand0center1max(),
                bhs::rand0center1max());
        }
        while (coord.Norm() > (T)1.0 || coord.Norm() < (T)rate);

        coordinates[i3    ] = coord.x;
        coordinates[i3 + 1] = coord.y;
        coordinates[i3 + 2] = coord.z;
    }

    for (quint64 i = 0; i < num * 3; ++i)
    {
        velocities[i] = (T)(rand0center1max() * 1.5e+4 * correct.m);
    }
}

template <typename T>
void Initializer3D<T>::initSunEarth()
{
    Correct correct(1.0e+11);
    m_engine->changeModelScale(correct.m);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = (T)(1.9891e+30 * correct.kg);
    coordinates[0] = coordinates[1] = coordinates[2] = 0.0;
    velocities[0] = velocities[1] = velocities[2] = 0.0;

    masses[1] = (T)(5.972e+24 * correct.kg);
    coordinates[3] = (T)(1.495978e+11 * correct.m);
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    velocities[3] = 0.0;
    velocities[4] = (T)(29780.0 * correct.m);
    velocities[5] = 0.0;
}

template <typename T>
void Initializer3D<T>::initEarthSun()
{
    Correct correct(1.0e+11);
    m_engine->changeModelScale(correct.m);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = (T)(5.972e+24 * correct.kg);
    coordinates[0] = 0.0;
    coordinates[1] = 0.0;
    coordinates[2] = 0.0;
    velocities[0] = 0.0;
    velocities[1] = 0.0;
    velocities[2] = (T)(29780.0 * correct.m);

    masses[1] = (T)(1.9891e+30 * correct.kg);
    coordinates[3] = (T)(1.495978e+11 * correct.m);
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    velocities[3] = 0.0;
    velocities[4] = 0.0;
    velocities[5] = 0.0;
}

template <typename T>
void Initializer3D<T>::initEarthMoon()
{
    Correct correct(4.0e+8);
    m_engine->changeModelScale(correct.m);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = (T)(5.972e+24 * correct.kg);
    coordinates[0] = 0.0;
    coordinates[1] = 0.0;
    coordinates[2] = 0.0;
    velocities[0] = 0.0;
    velocities[1] = (T)(-12.5 * correct.m);;
    velocities[2] = 0.0;

    masses[1] = (T)(7.347673e+22 * correct.kg);
    coordinates[3] = (T)(3.844e+8 * correct.m);
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    velocities[3] = 0.0;
    velocities[4] = (T)(1022.0 * correct.m);
    velocities[5] = 0.0;
}

template <typename T>
void Initializer3D<T>::initSunEarthVenus()
{
    Correct correct(1.0e+11);
    m_engine->changeModelScale(correct.m);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = (T)(1.9891e+30 * correct.kg);
    coordinates[0] = 0.0;
    coordinates[1] = 0.0;
    coordinates[2] = 0.0;
    velocities[0] = 0.0;
    velocities[1] = 0.0;
    velocities[2] = 0.0;

    masses[1] = (T)(5.972e+24 * correct.kg);
    coordinates[3] = (T)(1.495978e+11 * correct.m);
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    velocities[3] = 0.0;
    velocities[4] = 0.0;
    velocities[5] = (T)(29780.0 * correct.m);

    masses[2] = (T)(4.869e+24 * correct.kg);
    coordinates[6] = (T)(1.0820893e+11 * correct.m);
    coordinates[7] = 0.0;
    coordinates[8] = 0.0;
    velocities[6] = 0.0;
    velocities[7] = 0.0;
    velocities[8] = (T)(35021.4 * correct.m);
}

template <typename T>
void Initializer3D<T>::initTestSamePosition()
{
    m_engine->changeModelScale(1.0);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = 1.0;
    coordinates[0] = (T)0.1;
    coordinates[1] = (T)0.2;
    coordinates[2] = (T)0.3;
    velocities[0] = 0.0;
    velocities[1] = 0.0;
    velocities[2] = 0.0;

    masses[1] = 2.0;
    coordinates[3] = (T)0.1;
    coordinates[4] = (T)0.2;
    coordinates[5] = (T)0.3;
    velocities[3] = 0.0;
    velocities[4] = 0.0;
    velocities[5] = 0.0;
}
