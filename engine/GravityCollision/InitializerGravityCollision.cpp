#include "InitializerGravityCollision.h"

template class InitializerGravityCollision<float>;
template class InitializerGravityCollision<double>;

template <typename T>
void InitializerGravityCollision<T>::initRandomCube()
{
    m_3d.initRandomCube();
}

template <typename T>
void InitializerGravityCollision<T>::initRandomSphere(const double rate)
{
    m_3d.initRandomSphere(rate);
}

template <typename T>
void InitializerGravityCollision<T>::initCustom()
{
    m_3d.initCustom();
}
