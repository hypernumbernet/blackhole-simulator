#pragma once

#include "engine/3D/Initializer3D.h"

template <typename T>
class InitializerGravityCollision : protected AbstractInitializer
{

public:

    InitializerGravityCollision(
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
