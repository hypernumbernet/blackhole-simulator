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
    m_3d.initCustom();
    fromInitializer3D();
}
