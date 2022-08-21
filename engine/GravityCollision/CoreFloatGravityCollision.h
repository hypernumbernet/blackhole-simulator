#pragma once

#include "engine/abstractenginecorefloat.h"

class CoreFloatGravityCollision : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    explicit CoreFloatGravityCollision(
            AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(
            AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreFloatGravityCollision(engine, threadNumber);
    }

public slots:
    void calculateTimeProgress() const
    {
        resultReady();
    }

    void calculateInteraction() const
    {
        resultReady();
    }
};
