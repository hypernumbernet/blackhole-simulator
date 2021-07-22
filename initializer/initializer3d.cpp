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
            masses[i] = T(bhs::rand0to1() * m_sim.massAvg * 2.0 * correct.kg);
        }
    } else {
        for (quint64 i = 0; i < num; ++i)
        {
            masses[i] = m_sim.massAvg;
        }
    }
    for (quint64 i = 0; i < num * 3; ++i)
    {
        coordinates[i] = T(bhs::rand0center1max() * m_sim.scale * correct.m);
    }
    for (quint64 i = 0; i < num * 3; ++i)
    {
        velocities[i] = T(bhs::rand0center1max() * m_sim.speed * correct.m);
    }
    setRotation();
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
            masses[i] = T(bhs::rand0to1() * m_sim.massAvg * 2.0 * correct.kg);
        }
    } else {
        for (quint64 i = 0; i < num; ++i)
        {
            masses[i] = m_sim.massAvg * correct.kg;
        }
    }

    quint64 i3;
    Vector3<T> coord;
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
        while (coord.norm() > (T)1.0 || coord.norm() < (T)rate);

        coordinates[i3    ] = coord.x();
        coordinates[i3 + 1] = coord.y();
        coordinates[i3 + 2] = coord.z();
    }

    for (quint64 i = 0; i < num * 3; ++i)
    {
        velocities[i] = T(bhs::rand0center1max() * m_sim.speed * correct.m);
    }
    setRotation();
}

template <typename T>
void Initializer3D<T>::initSunEarth()
{
    Correct correct(1.0e+11);
    m_engine->changeModelScale(correct.m);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = T(1.9891e+30 * correct.kg);
    coordinates[0] = coordinates[1] = coordinates[2] = 0.0;
    velocities[0] = velocities[1] = velocities[2] = 0.0;

    masses[1] = T(5.972e+24 * correct.kg);
    coordinates[3] = T(1.495978e+11 * correct.m);
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    velocities[3] = 0.0;
    velocities[4] = T(29780.0 * correct.m);
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

    masses[0] = T(5.972e+24 * correct.kg);
    coordinates[0] = 0.0;
    coordinates[1] = 0.0;
    coordinates[2] = 0.0;
    velocities[0] = 0.0;
    velocities[1] = 0.0;
    velocities[2] = T(29780.0 * correct.m);

    masses[1] = T(1.9891e+30 * correct.kg);
    coordinates[3] = T(1.495978e+11 * correct.m);
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

    masses[0] = T(5.972e+24 * correct.kg);
    coordinates[0] = 0.0;
    coordinates[1] = 0.0;
    coordinates[2] = 0.0;
    velocities[0] = 0.0;
    velocities[1] = T(-12.5 * correct.m);;
    velocities[2] = 0.0;

    masses[1] = T(7.347673e+22 * correct.kg);
    coordinates[3] = T(3.844e+8 * correct.m);
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    velocities[3] = 0.0;
    velocities[4] = T(1022.0 * correct.m);
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

    masses[0] = T(1.9891e+30 * correct.kg);
    coordinates[0] = 0.0;
    coordinates[1] = 0.0;
    coordinates[2] = 0.0;
    velocities[0] = 0.0;
    velocities[1] = 0.0;
    velocities[2] = 0.0;

    masses[1] = T(5.972e+24 * correct.kg);
    coordinates[3] = T(1.495978e+11 * correct.m);
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    velocities[3] = 0.0;
    velocities[4] = 0.0;
    velocities[5] = T(29780.0 * correct.m);

    masses[2] = T(4.869e+24 * correct.kg);
    coordinates[6] = T(1.0820893e+11 * correct.m);
    coordinates[7] = 0.0;
    coordinates[8] = 0.0;
    velocities[6] = 0.0;
    velocities[7] = 0.0;
    velocities[8] = T(35021.4 * correct.m);
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

template <typename T>
void Initializer3D<T>::initSunMercury()
{
    Correct correct(5.0e+10);
    m_engine->changeModelScale(correct.m);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = T(1.9891e+30 * correct.kg);
    coordinates[0] = coordinates[1] = coordinates[2] = 0.0;
    velocities[0] = velocities[1] = velocities[2] = 0.0;

    // https://nssdc.gsfc.nasa.gov/planetary/factsheet/mercuryfact.html
    masses[1] = T(3.3011e+23 * correct.kg);
    coordinates[3] = 0.0;
    coordinates[4] = 0.0;
    coordinates[5] = T(4.6002e+10 * correct.m); // Perihelion
    velocities[3] = T(58980.0 * correct.m); // Max. orbital velocity
    velocities[4] = 0.0;
    velocities[5] = 0.0;
}

template <typename T>
void Initializer3D<T>::setRotation()
{
    quint64 num = m_engine->numberOfParticle();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();
    T time = m_engine->timePerFrame();
    double radian = degreeToRadian(m_sim.rotation);
    radian *= double(time);
    T sn = T(sin(radian));
    T cn = T(cos(radian));

    for (quint64 i = 0; i < num; ++i)
    {
        quint64 i3 = i * 3;
        T x1 = coordinates[i3    ];
        T z1 = coordinates[i3 + 2];
        T x2 = cn * x1 + sn * z1;
        T z2 = -sn * x1 + cn * z1;
        velocities[i3    ] += (x2 - x1) / time;
        velocities[i3 + 2] += (z2 - z1) / time;
    }
}
