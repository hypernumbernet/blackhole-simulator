#include "initializer3d4dr1.h"

template class Initializer3D4DR1<float>;
template class Initializer3D4DR1<double>;

template <typename T>
void Initializer3D4DR1<T>::initRandomCube()
{
    m_3d.initRandomCube();
    fromInitializer3D();
}

template <typename T>
void Initializer3D4DR1<T>::initRandomSphere(const double rate)
{
    m_3d.initRandomSphere(rate);
    fromInitializer3D();
}

template <typename T>
void Initializer3D4DR1<T>::initCustom()
{
}

template <typename T>
void Initializer3D4DR1<T>::initSunEarth()
{
    m_3d.initSunEarth();
    fromInitializer3D();
}

template <typename T>
void Initializer3D4DR1<T>::initEarthSun()
{
    m_3d.initEarthSun();
    fromInitializer3D();
}

template <typename T>
void Initializer3D4DR1<T>::initEarthMoon()
{
    m_3d.initEarthMoon();
    fromInitializer3D();
}

template <typename T>
void Initializer3D4DR1<T>::initSunEarthVenus()
{
    m_3d.initSunEarthVenus();
    fromInitializer3D();
}

template <typename T>
void Initializer3D4DR1<T>::initTestSamePosition()
{
    m_3d.initTestSamePosition();
    fromInitializer3D();
}

template <typename T>
void Initializer3D4DR1<T>::initSunMercury()
{
    m_3d.initSunMercury();
    fromInitializer3D();
}
