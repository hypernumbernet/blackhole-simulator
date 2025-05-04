#include "InitializerRelativity4.h"

template class InitializerRelativity4<float>;
template class InitializerRelativity4<double>;

template <typename T>
void InitializerRelativity4<T>::initRandomCube()
{
    m_3d.initRandomCube();
    fromInitializer3D();
}

template <typename T>
void InitializerRelativity4<T>::initRandomSphere(const double rate)
{
    m_3d.initRandomSphere(rate);
    fromInitializer3D();
}

template <typename T>
void InitializerRelativity4<T>::initCustom()
{
    m_3d.initCustom();
    fromInitializer3D();
}
