#include "InitializerRelativity1.h"

template class InitializerRelativity1<float>;
template class InitializerRelativity1<double>;

template <typename T>
void InitializerRelativity1<T>::initRandomCube()
{
    m_3d.initRandomCube();
    fromInitializer3D();
}

template <typename T>
void InitializerRelativity1<T>::initRandomSphere(const double rate)
{
    m_3d.initRandomSphere(rate);
    fromInitializer3D();
}

template <typename T>
void InitializerRelativity1<T>::initCustom()
{
    m_3d.initCustom();
    fromInitializer3D();
}
