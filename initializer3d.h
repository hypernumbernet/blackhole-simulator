#pragma once

#include "abstractinitializer.h"

class Initializer3D : private AbstractInitializer
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
