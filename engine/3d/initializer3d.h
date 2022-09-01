#pragma once

#include "engine/abstractinitializer.h"
#include "engine/abstractnbodyengine.h"

template <typename T> class Initializer3D4D;
template <typename T> class Initializer3D4DR1;
template <typename T> class Initializer4D3D;
template <typename T> class InitializerGravityCollision;
template <typename T> class InitializerGravityElasticity;

template <typename T>
class Initializer3D : protected AbstractInitializer
{
    friend Initializer3D4D<T>;
    friend Initializer3D4DR1<T>;
    friend Initializer4D3D<T>;
    friend InitializerGravityCollision<T>;
    friend InitializerGravityElasticity<T>;

public:

    Initializer3D(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : AbstractInitializer(sim)
        , m_engine(engine)
    {
    }

private:
    void initRandomCube() override;
    void initRandomSphere(double) override;
    void initCustom() override;

    void setRotation();
    AbstractNBodyEngine<T>* const m_engine;
};
