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
    m_3d.initCustom();
    fromInitializer3D();
}
