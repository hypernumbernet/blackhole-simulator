#include "initializer3d4d.h"

template class Initializer3D4D<float>;
template class Initializer3D4D<double>;

template <typename T>
void Initializer3D4D<T>::initRandamCube()
{
    m_engine->changeModelScale(1.0 / this->m_sim.scale);

    const quint64 num = m_engine->numberOfParticle();
    T* const coordinates = m_engine->coordinates();
    T* const velocities = m_engine->velocities();

    T* const masses = m_engine->masses();
    if (this->m_sim.massRandom)
    {
        for (quint64 i = 0; i < num; ++i)
        {
            masses[i] = randf<T>() * this->m_sim.massAvg * 2.0;
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

    bhs::Vector3<T> axis;
    T angle;
    quint64 i4;
    for (quint64 i = 0; i < num; ++i)
    {
        axis.set(randf<T>(), randf<T>(), randf<T>());
        axis.Normalize();
        angle = bhs::rand0center1maxf() * 1.5e+4 * AbstractNBodyEngine<T>::VANGLE;
        auto vq = bhs::Quaternion<T>::Exp(axis * angle);
        i4 = i * 4;
        velocities[i4    ] = vq.i0;
        velocities[i4 + 1] = vq.i1;
        velocities[i4 + 2] = vq.i2;
        velocities[i4 + 3] = vq.i3;
    }
}

template <typename T>
void Initializer3D4D<T>::initRandamSphere(const double rate)
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
            masses[i] = randf<T>() * this->m_sim.massAvg * 2.0;
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
        while (cood.Norm() > 1.0 || cood.Norm() < (T)rate);

        cood *= (T)this->m_sim.scale;
        coordinates[i3    ] = cood.x;
        coordinates[i3 + 1] = cood.y;
        coordinates[i3 + 2] = cood.z;
    }

    bhs::Vector3<T> axis;
    T angle;
    quint64 i4;
    for (quint64 i = 0; i < num; ++i)
    {
        axis.set(randf<T>(), randf<T>(), randf<T>());
        axis.Normalize();
        angle = bhs::rand0center1maxf() * 1.5e+4 * AbstractNBodyEngine<T>::VANGLE;
        auto vq = bhs::Quaternion<T>::Exp(axis * angle);
        i4 = i * 4;
        velocities[i4    ] = vq.i0;
        velocities[i4 + 1] = vq.i1;
        velocities[i4 + 2] = vq.i2;
        velocities[i4 + 3] = vq.i3;
    }
}

template <typename T>
void Initializer3D4D<T>::initSunEarth()
{
    m_engine->changeModelScale((T)1.0e-11);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = (T)1.9891e+30;
    coordinates[0] = coordinates[1] = coordinates[2] = 0.0;
    velocities[0] = 1.0;
    velocities[1] = 0.0;
    velocities[2] = 0.0;
    velocities[3] = 0.0;

    masses[1] = (T)5.972e+24;
    coordinates[3] = (T)1.495978e+11;
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    auto q = fromDirectionAndSpeed(0.0, 1.0, 0.0, 29780.0);
    embedQuaternionToArray(q, velocities, 4);
}

template <typename T>
void Initializer3D4D<T>::initEarthSun()
{
    m_engine->changeModelScale((T)1.0e-11);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = (T)5.972e+24;
    coordinates[0] = 0.0;
    coordinates[1] = 0.0;
    coordinates[2] = 0.0;
    auto q = fromDirectionAndSpeed(0.0, 1.0, 0.0, 29780.0);
    embedQuaternionToArray(q, velocities, 0);

    masses[1] = (T)1.9891e+30;
    coordinates[3] = (T)1.495978e+11;
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    velocities[4] = 1.0;
    velocities[5] = 0.0;
    velocities[6] = 0.0;
    velocities[7] = 0.0;
}

template <typename T>
void Initializer3D4D<T>::initSunEarthAu()
{
    m_engine->changeModelScale((T)1.0e-11);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = (T)1.9891e+30;
    coordinates[0] = coordinates[1] = coordinates[2] = 0.0;
    velocities[0] = 1.0;
    velocities[1] = 0.0;
    velocities[2] = 0.0;
    velocities[3] = 0.0;

    masses[1] = (T)5.972e+24;
    coordinates[3] = (T)1.495978e+11;
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    auto q = fromDirectionAndSpeed(0.0, 1.0, 0.0, 29780.0);
    embedQuaternionToArray(q, velocities, 4);
}

template <typename T>
void Initializer3D4D<T>::initEarthMoon()
{
    m_engine->changeModelScale((T)0.25e-8);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = (T)5.972e+24;
    coordinates[0] = 0.0;
    coordinates[1] = 0.0;
    coordinates[2] = 0.0;
    velocities[0] = 1.0;
    velocities[1] = 0.0;
    velocities[2] = 0.0;
    velocities[3] = 0.0;

    masses[1] = (T)7.347673e+22;
    coordinates[3] = (T)3.844e+8;
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    auto q = fromDirectionAndSpeed(0.0, 1.0, 0.0, 1022.0);
    embedQuaternionToArray(q, velocities, 4);
}

template <typename T>
void Initializer3D4D<T>::initSunEarthVenus()
{
    m_engine->changeModelScale((T)1.0e-11);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = (T)1.9891e+30;
    coordinates[0] = 0.0;
    coordinates[1] = 0.0;
    coordinates[2] = 0.0;
    velocities[0] = 1.0;
    velocities[1] = 0.0;
    velocities[2] = 0.0;
    velocities[3] = 0.0;

    masses[1] = (T)5.972e+24;
    coordinates[3] = (T)1.495978e+11;
    coordinates[4] = 0.0;
    coordinates[5] = 0.0;
    auto earth = fromDirectionAndSpeed(0.0, 0.0, 1.0, (T)29780.0);
    embedQuaternionToArray(earth, velocities, 4);

    masses[2] = (T)4.869e+24;
    coordinates[6] = (T)1.0820893e+11;
    coordinates[7] = 0.0;
    coordinates[8] = 0.0;
    auto venus = fromDirectionAndSpeed(0.0, 0.0, 1.0, (T)35021.4);
    embedQuaternionToArray(venus, velocities, 8);
}

template <typename T>
void Initializer3D4D<T>::initTestSamePosition()
{
    m_engine->changeModelScale(1.0);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    masses[0] = 1.0e+10;
    coordinates[0] = (T)0.1;
    coordinates[1] = (T)0.2;
    coordinates[2] = (T)0.3;
    velocities[0] = 1.0;
    velocities[1] = 0.0;
    velocities[2] = 0.0;
    velocities[3] = 0.0;

    masses[1] = 2.0e+10;
    coordinates[3] = (T)0.1;
    coordinates[4] = (T)0.2;
    coordinates[5] = (T)0.3;
    velocities[4] = 1.0;
    velocities[5] = 0.0;
    velocities[6] = 0.0;
    velocities[7] = 0.0;
}
