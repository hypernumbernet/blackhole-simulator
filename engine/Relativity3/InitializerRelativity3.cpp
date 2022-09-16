#include "InitializerRelativity3.h"

template class InitializerRelativity3<float>;
template class InitializerRelativity3<double>;

template <typename T>
void InitializerRelativity3<T>::initRandomCube()
{
    m_3d.initRandomCube();
    fromInitializer3D();
}

template <typename T>
void InitializerRelativity3<T>::initRandomSphere(const double rate)
{
    m_3d.initRandomSphere(rate);
    fromInitializer3D();
}

template <typename T>
void InitializerRelativity3<T>::initCustom()
{
    m_3d.initCustom();
    fromInitializer3D();
}
