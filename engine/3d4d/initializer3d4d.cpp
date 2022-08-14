#include "initializer3d4d.h"

template class Initializer3D4D<float>;
template class Initializer3D4D<double>;

template <typename T>
void Initializer3D4D<T>::initRandomCube()
{
    m_3d.initRandomCube();
    fromInitializer3D();
}

template <typename T>
void Initializer3D4D<T>::initRandomSphere(const double rate)
{
    m_3d.initRandomSphere(rate);
    fromInitializer3D();
}

template <typename T>
void Initializer3D4D<T>::initCustom()
{
}

template <typename T>
void Initializer3D4D<T>::initSunEarth()
{
    m_3d.initSunEarth();
    fromInitializer3D();
}

template <typename T>
void Initializer3D4D<T>::initEarthSun()
{
    m_3d.initEarthSun();
    fromInitializer3D();
}

template <typename T>
void Initializer3D4D<T>::initEarthMoon()
{
    m_3d.initEarthMoon();
    fromInitializer3D();
}

template <typename T>
void Initializer3D4D<T>::initSunEarthVenus()
{
    m_3d.initSunEarthVenus();
    fromInitializer3D();
}

template <typename T>
void Initializer3D4D<T>::initTestSamePosition()
{
    m_3d.initTestSamePosition();
    fromInitializer3D();
}

template <typename T>
void Initializer3D4D<T>::initSunMercury()
{
    m_3d.initSunMercury();
    fromInitializer3D();
}
