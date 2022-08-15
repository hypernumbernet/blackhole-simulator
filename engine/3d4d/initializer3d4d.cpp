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
    m_3d.initCustom();
    fromInitializer3D();
}
