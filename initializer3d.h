#pragma once

#include "abstractinitializer.h"
#include "quaternion.h"

using namespace bhs;

template <typename T>
class Initializer3D : private AbstractInitializer<T>
{
public:
    using AbstractInitializer<T>::AbstractInitializer;

protected:
    void initRandamCube() override;
    void initRandamSphere(T) override;
    void initSunEarth() override;
    void initEarthSun() override;
    void initEarthMoon() override;
    void initSunEarthVenus() override;
    void initTestSamePosition() override;
};
