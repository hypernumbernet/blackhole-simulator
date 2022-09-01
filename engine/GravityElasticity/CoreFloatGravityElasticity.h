#pragma once

#include "engine/abstractenginecorefloat.h"

class CoreFloatGravityElasticity : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    explicit CoreFloatGravityElasticity(
            AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(
            AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreFloatGravityElasticity(engine, threadNumber);
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
