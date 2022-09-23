#pragma once

#include "engine/AbstractInitializer.h"
#include "engine/AbstractNBodyEngine.h"

template <typename T>
class Initializer3D : protected AbstractInitializer
{

public:

    Initializer3D(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : AbstractInitializer(sim)
        , m_engine(engine)
    {
    }

    void initRandomCube() override;
    void initRandomSphere(double) override;
    void initCustom() override;

private:
    void setRotation();
    AbstractNBodyEngine<T>* const m_engine;
};
