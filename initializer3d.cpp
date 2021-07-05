#include "initializer3d.h"

template class Initializer3D<float>;
template class Initializer3D<double>;

template <typename T>
void Initializer3D<T>::initRandamCube()
{
    m_engine->changeModelScale(1.0 / this->m_sim.scale);

    quint64 num = m_engine->numberOfParticle();
    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    if (this->m_sim.massRandom)
    {
        for (quint64 i = 0; i < num; ++i)
        {
            masses[i] = randf<T>() * this->m_sim.massAvg * (T)2.0;
        }
    } else {
        for (quint64 i = 0; i < num; ++i)
        {
            masses[i] = this->m_sim.massAvg;
        }
    }
    for (quint64 i = 0; i < num * 3; ++i)
    {
        coordinates[i] = bhs::rand0center1maxf() * (T)this->m_sim.scale;
    }
    for (quint64 i = 0; i < num * 3; ++i)
    {
        velocities[i] = bhs::rand0center1maxf() * (T)1.5e+4;
    }

//    masses[0] = 2.0e+30f;
//    coordinates[0] = 0.0f;
//    coordinates[1] = 0.0f;
//    coordinates[2] = 0.0f;
}

template <typename T>
void Initializer3D<T>::initRandamSphere(const double rate)
{
    m_engine->changeModelScale((T)1.0 / this->m_sim.scale);

    quint64 num = m_engine->numberOfParticle();
    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    if (this->m_sim.massRandom)
    {
        for (quint64 i = 0; i < num; ++i)
        {
            masses[i] = randf<T>() * this->m_sim.massAvg * (T)2.0;
        }
    } else {
        for (quint64 i = 0; i < num; ++i)
        {
            masses[i] = this->m_sim.massAvg;
        }
    }

    quint64 i3;
    bhs::Vector3<T> cood;
    for (quint64 i = 0; i < num; ++i)
    {
        i3 = i * 3;

        do
        {
            cood.set(
                bhs::rand0center1maxf(),
                bhs::rand0center1maxf(),
                bhs::rand0center1maxf());
        }
        while (cood.Norm() > (T)1.0 || cood.Norm() < (T)rate);

        cood *= (T)this->m_sim.scale;
        coordinates[i3    ] = cood.x;
        coordinates[i3 + 1] = cood.y;
        coordinates[i3 + 2] = cood.z;
    }

    for (quint64 i = 0; i < num * 3; ++i)
    {
        velocities[i] = randf<T>() * (T)3.0e+4 - (T)1.5e+4;
    }
}

template <typename T>
void Initializer3D<T>::initSunEarth()
{
    m_engine->changeModelScale((T)1.0e-11);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = (T)1.9891e+30;
    coordinates[0] = coordinates[1] = coordinates[2] = 0.0;
    velocities[0] = velocities[1] = velocities[2] = 0.0;

    masses[1] = (T)5.972e+24;
    coordinates[3] = (T)1.495978e+11;
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    velocities[3] = 0.0;
    velocities[4] = 29780.0;
    velocities[5] = 0.0;
}

template <typename T>
void Initializer3D<T>::initEarthSun()
{
    m_engine->changeModelScale((T)1.0e-11);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = (T)5.972e+24;
    coordinates[0] = 0.0;
    coordinates[1] = 0.0;
    coordinates[2] = 0.0;
    velocities[0] = 0.0;
    velocities[1] = 0.0;
    velocities[2] = 29780.0;

    masses[1] = (T)1.9891e+30;
    coordinates[3] = (T)1.495978e+11;
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    velocities[3] = 0.0;
    velocities[4] = 0.0;
    velocities[5] = 0.0;
}

template <typename T>
void Initializer3D<T>::initSunEarthAu()
{
    m_engine->changeModelScale((T)1.0e-11);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = (T)1.9891e+30;
    coordinates[0] = coordinates[1] = coordinates[2] = 0.0;
    velocities[0] = velocities[1] = velocities[2] = 0.0;

    masses[1] = (T)5.972e+24;
    coordinates[3] = (T)1.495978e+11;
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    velocities[3] = 0.0;
    velocities[4] = 29780.0;
    velocities[5] = 0.0;
}

template <typename T>
void Initializer3D<T>::initEarthMoon()
{
    m_engine->changeModelScale((T)0.25e-8);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = (T)5.972e+24;
    coordinates[0] = 0.0;
    coordinates[1] = 0.0;
    coordinates[2] = 0.0;
    velocities[0] = 0.0;
    velocities[1] = 0.0;
    velocities[2] = 0.0;

    masses[1] = (T)7.347673e+22;
    coordinates[3] = 3.844e+8;
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    velocities[3] = 0.0;
    velocities[4] = 1022.0;
    velocities[5] = 0.0;
}

template <typename T>
void Initializer3D<T>::initSunEarthVenus()
{
    m_engine->changeModelScale(1.0e-11f);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = (T)1.9891e+30;
    coordinates[0] = 0.0;
    coordinates[1] = 0.0;
    coordinates[2] = 0.0;
    velocities[0] = 0.0;
    velocities[1] = 0.0;
    velocities[2] = 0.0;

    masses[1] = (T)5.972e+24;
    coordinates[3] = (T)1.495978e+11;
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    velocities[3] = 0.0;
    velocities[4] = 0.0;
    velocities[5] = 29780.0;

    masses[2] = (T)4.869e+24;
    coordinates[6] = (T)1.0820893e+11;
    coordinates[7] = 0.0;
    coordinates[8] = 0.0;
    velocities[6] = 0.0;
    velocities[7] = 0.0;
    velocities[8] = (T)35021.4;
}

template <typename T>
void Initializer3D<T>::initTestSamePosition()
{
    m_engine->changeModelScale(1.0);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = 1.0e+10;
    coordinates[0] = (T)0.1;
    coordinates[1] = (T)0.2;
    coordinates[2] = (T)0.3;
    velocities[0] = 0.0;
    velocities[1] = 0.0;
    velocities[2] = 0.0;

    masses[1] = 2.0e+10;
    coordinates[3] = (T)0.1;
    coordinates[4] = (T)0.2;
    coordinates[5] = (T)0.3;
    velocities[3] = 0.0;
    velocities[4] = 0.0;
    velocities[5] = 0.0;
}
