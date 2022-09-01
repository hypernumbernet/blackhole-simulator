#pragma once

#include "engine/3d/initializer3d.h"

template <typename T>
class InitializerGravityElasticity : protected AbstractInitializer
{

public:

    InitializerGravityElasticity(
            const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : AbstractInitializer(sim)
        , m_engine(engine)
        , m_3d(sim, engine)
    {
    }

private:
    void initRandomCube() override;
    void initRandomSphere(double) override;
    void initCustom() override;

    AbstractNBodyEngine<T>* const m_engine;

    Initializer3D<T> m_3d;
};
