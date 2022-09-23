#include "InitializerUniverse1.h"

template class InitializerUniverse1<float>;
template class InitializerUniverse1<double>;

template <typename T>
void InitializerUniverse1<T>::initRandomCube()
{
    m_3d.initRandomCube();
    fromInitializer3D();
}

template <typename T>
void InitializerUniverse1<T>::initRandomSphere(const double rate)
{
    m_3d.initRandomSphere(rate);
    fromInitializer3D();
}

template <typename T>
void InitializerUniverse1<T>::initCustom()
{
    m_3d.initCustom();
    fromInitializer3D();
}
