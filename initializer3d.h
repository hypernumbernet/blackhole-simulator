#pragma once

#include "abstractinitializer.h"

class Initializer3D : private AbstractInitializer
{
public:
    using AbstractInitializer::AbstractInitializer;

protected:
    void initParticlesRandam(AbstractNBodyEngine* const) override;
    void initSunEarth(AbstractNBodyEngine* const) override;
    void initEarthSun(AbstractNBodyEngine* const) override;
    void initEarthMoon(AbstractNBodyEngine* const) override;
    void initSunEarthVenus(AbstractNBodyEngine* const) override;
    void initTestSamePosition(AbstractNBodyEngine* const) override;
};
