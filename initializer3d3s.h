#pragma once

#include "abstractinitializer.h"
#include "quaternion.h"

class Initializer3D3S : public AbstractInitializer
{
public:
    using AbstractInitializer::AbstractInitializer;

protected:
    void initParticlesRandam() override;
    void initSunEarth() override;
    void initEarthSun() override;
    void initEarthMoon() override;
    void initSunEarthVenus() override;
    void initTestSamePosition() override;
};
