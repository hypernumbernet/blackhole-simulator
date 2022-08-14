#include "initializer4d3d.h"

template class Initializer4D3D<float>;
template class Initializer4D3D<double>;

template <typename T>
void Initializer4D3D<T>::initRandomCube()
{
    m_3d.initRandomCube();
    fromInitializer3D();
}

template <typename T>
void Initializer4D3D<T>::initRandomSphere(const double rate)
{
    m_3d.initRandomSphere(rate);
    fromInitializer3D();
}

template <typename T>
void Initializer4D3D<T>::initCustom()
{
}

template <typename T>
void Initializer4D3D<T>::initSunEarth()
{
    m_3d.initSunEarth();
    fromInitializer3D();
}

template <typename T>
void Initializer4D3D<T>::initEarthSun()
{
    m_3d.initEarthSun();
    fromInitializer3D();
}

template <typename T>
void Initializer4D3D<T>::initEarthMoon()
{
    m_3d.initEarthMoon();
    fromInitializer3D();
}

template <typename T>
void Initializer4D3D<T>::initSunEarthVenus()
{
    m_3d.initSunEarthVenus();
    fromInitializer3D();
}

template <typename T>
void Initializer4D3D<T>::initTestSamePosition()
{
    m_3d.initTestSamePosition();
    fromInitializer3D();
}

template <typename T>
void Initializer4D3D<T>::initSunMercury()
{
    m_3d.initSunMercury();
    fromInitializer3D();
}
