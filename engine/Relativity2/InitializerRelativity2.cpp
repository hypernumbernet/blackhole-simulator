#include "InitializerRelativity2.h"

template class InitializerRelativity2<float>;
template class InitializerRelativity2<double>;

template <typename T>
void InitializerRelativity2<T>::initRandomCube()
{
    m_3d.initRandomCube();
    fromInitializer3D();
}

template <typename T>
void InitializerRelativity2<T>::initRandomSphere(const double rate)
{
    m_3d.initRandomSphere(rate);
    fromInitializer3D();
}

template <typename T>
void InitializerRelativity2<T>::initCustom()
{
    m_3d.initCustom();
    fromInitializer3D();
}
