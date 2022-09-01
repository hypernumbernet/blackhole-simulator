#include "InitializerGravityElasticity.h"

template class InitializerGravityElasticity<float>;
template class InitializerGravityElasticity<double>;

template <typename T>
void InitializerGravityElasticity<T>::initRandomCube()
{
    m_3d.initRandomCube();
}

template <typename T>
void InitializerGravityElasticity<T>::initRandomSphere(const double rate)
{
    m_3d.initRandomSphere(rate);
}

template <typename T>
void InitializerGravityElasticity<T>::initCustom()
{
    m_3d.initCustom();
}
