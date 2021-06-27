#pragma once

#include "abstractinitializer.h"
#include "quaternion.h"

class Initializer3D : private AbstractInitializer
{
public:
    using AbstractInitializer::AbstractInitializer;

protected:
    void initRandamCube() override;
    void initRandamSphere(const float) override;
    void initSunEarth() override;
    void initEarthSun() override;
    void initEarthMoon() override;
    void initSunEarthVenus() override;
    void initTestSamePosition() override;
};
