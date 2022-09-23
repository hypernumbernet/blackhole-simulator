#include "initializer4d3d.h"

template class InitializerUniverse2<float>;
template class InitializerUniverse2<double>;

template <typename T>
void InitializerUniverse2<T>::initRandomCube()
{
    m_3d.initRandomCube();
    fromInitializer3D();
}

template <typename T>
void InitializerUniverse2<T>::initRandomSphere(const double rate)
{
    m_3d.initRandomSphere(rate);
    fromInitializer3D();
}

template <typename T>
void InitializerUniverse2<T>::initCustom()
{
    m_3d.initCustom();
    fromInitializer3D();
}
