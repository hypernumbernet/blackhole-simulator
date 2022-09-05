#pragma once

#include "engine/abstractenginecorefloat.h"

class CoreFloat3D4D : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    CoreFloat3D4D(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
    {
    }

    static AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreFloat3D4D(engine, threadNumber);
    }

    bool hasRangeInteraction() const override
    {
        return m_hasRangeTimeProgress;
    }

public slots:
    void calculateTimeProgress() const override
    {
        resultReady();
    }

    void calculateInteraction() const override
    {
        resultReady();
    }
};
