#include "initializer3d.h"

//####################################################################
//
// To avoid a float overflow and speed up the calculations.
//
// Convert
//
// 10^10 (m) = 10000000000 (m) = 1 (length here)
// 10^30 (kg) = 1 (mass here)
//
// G constant (m^3 kg^-1 s^-2) : -10-10-10+30 = 0 (not change)
//
// You should choose the right scale for your calculations.
// The "Correct" class help this Conversions.
//
//####################################################################

template class Initializer3D<float>;
template class Initializer3D<double>;

template <typename T>
void Initializer3D<T>::initRandomCube()
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
void Initializer3D<T>::initRandomSphere(const double rate)
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
void Initializer3D<T>::initCustom()
{
    Correct correct(m_sim.custom.scale);
    m_engine->changeModelScale(correct.m);

    T* masses = m_engine->masses();
    T* coordinates = m_engine->coordinates();
    T* velocities = m_engine->velocities();

    int i = 0;
    for (const bhs::Particle& e : m_sim.custom.particles)
    {
        masses[i] = T(e.mass * correct.kg);
        int i3 = i * 3;
        coordinates[i3    ] = T(e.coordinate.x() * correct.m);
        coordinates[i3 + 1] = T(e.coordinate.y() * correct.m);
        coordinates[i3 + 2] = T(e.coordinate.z() * correct.m);
        velocities[i3    ] = T(e.velocity.x() * correct.m);
        velocities[i3 + 1] = T(e.velocity.y() * correct.m);
        velocities[i3 + 2] = T(e.velocity.z() * correct.m);
        ++i;
    }
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
